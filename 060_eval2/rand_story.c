#include "rand_story.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//step1
//read and parse the template file
void parse_template(char * line, catarray_t * cats, category_t * record, int reuse) {
  size_t idx = 0, cat_len = 0;
  char * cat = NULL;
  bool blank = false;

  //print the output when we read the line
  while (line[idx] != '\0') {
    if (line[idx] != '_' && !blank) {  //not in the blank
      printf("%c", line[idx]);
      idx += 1;
    }
    else if (line[idx] == '_' && !blank) {  //here is a new blank
      blank = true;
      idx += 1;
    }
    else if (line[idx] != '_' && blank) {  //in the blank
      //find the last '_' and get the length
      size_t start = idx;
      size_t tmp_i = 0;
      while (line[idx] != '\0' && line[idx] != '_') {
        cat_len++;
        idx++;
      }
      //get category
      cat = realloc(cat, (cat_len + 1) * sizeof(*cat));
      while (start < idx) {
        cat[tmp_i] = line[start];
        tmp_i++;
        start++;
      }
      cat[cat_len] = '\0';
    }
    else if (line[idx] == '_' && blank) {  //end of blank
      const char * word;
      //if cats is empty.
      if (cats == NULL)
        word = chooseWord(cat, cats);
      char * end_p;
      int tmp_num = strtoul(cat, &end_p, 10);
      int t_index;
      //the word in the blank is a category, but not number.
      if (*end_p != '\0' || tmp_num == 0) {
        tmp_num = 0;
        t_index = cats == NULL ? -1 : contain_cat(cats, cat);
      }
      //tmp_num >= 1 means cat is a number
      if (tmp_num >= 1) {
        if ((size_t)tmp_num > record->n_words) {
          fprintf(stderr, "Error template! Do not have enough words.\n");
          exit(EXIT_FAILURE);
        }
        //debugging: printf("cat:%s, t_idx=%d, t_num=%d \n", cat, t_index, tmp_num);
        word = My_Choose_Word(cat, cats, record, t_index, tmp_num);
      }  //category is number
      else if (t_index >= 0 && cats->arr[t_index].n_words) {
        word = chooseWord(cat, cats);
      }
      else {
        fprintf(stderr, "Other Error from My_Choose_Word!");
        exit(EXIT_FAILURE);
      }

      cat_len = 0;
      blank = false;
      idx++;
      printf("%s", word);

      //update varibles in record
      record->n_words++;
      record->words = realloc(record->words, sizeof(*record->words) * (record->n_words));
      char * tmp = malloc((strlen(word) + 1) * sizeof(*tmp));
      int j = 0;
      while (word[j] != 0) {
        tmp[j] = word[j];
        j++;
      }
      tmp[j] = '\0';
      record->words[record->n_words - 1] = tmp;

      if (t_index != -1 && !reuse) {
        if (tmp_num < 1 && cats->arr[t_index].n_words != 0)
          update_cats(cats, cat, word);
      }
    }
  }
  free(cat);

  if (blank == true) {  //no matching '_'
    fprintf(stderr, "Cannot match the '_': %s \n", line);
    exit(EXIT_FAILURE);
  }
}

//reading template and parse it
void read_template(FILE * f, catarray_t * cats, int reuse) {
  char * line = NULL;
  size_t size_ = 0;
  //init Record
  category_t * record = malloc(sizeof(*record));
  record->n_words = 0;
  record->name = "USED";
  record->words = NULL;
  //parse it!
  while (getline(&line, &size_, f) >= 0) {
    parse_template(line, cats, record, reuse);
  }
  free(line);
  //freeRecord
  for (size_t i = 0; i < record->n_words; i++) {
    free(record->words[i]);
  }
  free(record->words);
  free(record);
}

//step2
//parse the words.txt and get the word
char * parse_input_inst(char * line) {
  char * l_end = strchr(line, '\n');
  *l_end = '\0';
  char * w_start = strchr(line, ':');
  int len_ = 0;
  //get word
  if (w_start != NULL) {
    w_start += 1;
    len_ = l_end - w_start + 1;
    char * instance = malloc(sizeof(instance) * len_);
    int i = 0;
    while (w_start <= l_end) {
      instance[i] = *w_start;
      i++;
      w_start++;
    }
    return instance;
  }
  else {
    fprintf(stderr, "NO ':' in the line %s. \n", line);
    exit(EXIT_FAILURE);
  }
}

//parse the words.txt and get the category word
char * parse_input_cat(char * line) {
  char * cat_end = strchr(line, ':');
  char * cat_ins;
  int i = 0;
  //get category
  if (cat_end != NULL) {
    *cat_end = '\0';
    //char * cat_ins = strdup(line);
    int len_ = cat_end - &line[0] + 1;
    cat_ins = malloc(sizeof(cat_ins) * len_);
    while (i <= len_) {
      cat_ins[i] = line[i];
      i++;
    }
    return cat_ins;
  }
  else {
    fprintf(stderr, "There is no ':' in line: %s. \n", line);
    exit(EXIT_FAILURE);
  }
}

//find the index of category int cats[], if there is no category return -1
int contain_cat(catarray_t * cats, char * cat) {
  size_t idx = 0;
  while (idx < cats->n) {
    if (strcmp(cat, cats->arr[idx].name) == 0)
      return idx;
    idx++;
  }
  return -1;
}

//read the file and parse it
catarray_t * parse_cat_file(FILE * f) {
  char * line = NULL;
  size_t size_ = 0;
  catarray_t * cats = malloc(sizeof(*cats));
  cats->arr = NULL;
  cats->n = 0;
  while (getline(&line, &size_, f) >= 0) {
    char * inst = parse_input_inst(line);
    char * cat = parse_input_cat(line);
    int idx = contain_cat(cats, cat);
    //when cat is a new type
    if (!(contain_cat(cats, cat) + 1)) {
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = cat;
      cats->arr[cats->n].words = malloc(sizeof(*cats->arr[cats->n].words));
      cats->arr[cats->n].words[0] = inst;
      cats->arr[cats->n].n_words = 1;
      cats->n++;
    }
    else {  //when cat is an old type
            //we need free(cat);
      //we need a varibles to record whether is has recorded in cats structure
      bool has_word = false;
      for (size_t i = 0; i < cats->arr[idx].n_words; i++) {
        if (cat == cats->arr[idx].words[i]) {
          has_word = true;
          break;
        }
      }
      //if the word has been record, then go to the next loop
      if (has_word)
        continue;
      //else add a new word to the cats
      free(cat);
      cats->arr[idx].n_words++;
      cats->arr[idx].words = realloc(
          cats->arr[idx].words, cats->arr[idx].n_words * sizeof(*cats->arr[idx].words));
      cats->arr[idx].words[cats->arr[idx].n_words - 1] = inst;
    }
  }
  free(line);
  return cats;
}

//function to free the category
void freeCat(catarray_t * cats) {
  for (size_t i = 0; i < cats->n; i++) {
    for (size_t j = 0; j < cats->arr[i].n_words; j++) {
      free(cats->arr[i].words[j]);
    }
    free(cats->arr[i].words);
    free(cats->arr[i].name);
  }
  free(cats->arr);
  free(cats);
}

//step3
//choose word different from chooseword
//1.input category
//2.input number
const char * My_Choose_Word(char * cat,
                            catarray_t * cats,
                            category_t * record,
                            int t_index,
                            int tmp_num) {
  if (cats == NULL) {
    return chooseWord(cat, NULL);
  }
  if ((size_t)tmp_num <= record->n_words) {
    return record->words[record->n_words - tmp_num];
  }
  else if (t_index >= 0 && cats->arr[t_index].n_words) {
    return chooseWord(cat, cats);
  }
  else {
    fprintf(stderr, "Error from: doesn't contain enough words in category\n");
    exit(EXIT_FAILURE);
  }
}

//step4
//update cats, if there is word has been used
void update_cats(catarray_t * cats, char * cat, const char * word) {
  //calculate index
  int idx = contain_cat(cats, cat);
  size_t i = 0;
  size_t w_idx;
  //try to find the used word's index
  while (i < cats->arr[idx].n_words) {
    w_idx = strcmp(word, cats->arr[idx].words[i]) == 0 ? i : 0;
    i++;
  }

  //if word index is the last element, we can just remove it
  if (w_idx == cats->arr[idx].n_words - 1) {
    free(cats->arr[idx].words[w_idx]);
    cats->arr[idx].n_words--;
    return;
  }

  //if word index is not the last element, update it
  size_t k = w_idx;
  char * tmp;
  free(cats->arr[idx].words[w_idx]);
  while (k < cats->arr[idx].n_words - 1) {
    tmp = cats->arr[idx].words[k + 1];
    cats->arr[idx].words[k] = tmp;
    k++;
  }
  cats->arr[idx].n_words--;
}
