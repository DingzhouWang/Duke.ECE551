#include "rand_story.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//step1
void parse_template(char * line, catarray_t * cats, category_t * record, int reuse) {
  size_t idx = 0, cat_len = 0;
  char * cat = NULL;
  bool blank = false;

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
      //do something
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
      const char * word = My_Choose_Word(cat, cats, record);
      cat_len = 0;
      blank = false;
      idx++;
      printf("%s", word);
      record->n_words++;
      record->words = realloc(record->words, sizeof(*record->words) * (record->n_words));
      record->words[record->n_words - 1] = strdup(word);
      if (cat != NULL && reuse == 0) {
        // not track use
        //printf("%s", cat);
        int index = contain_cat(cats, cat);
        //printf("%d", index);
        if (atoi(cat) < 1 && index != -1 && cats->arr[index].n_words != 0)
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

void freeRecord(category_t * record) {
  for (size_t i = 0; i < record->n_words; i++) {
    free(record->words[i]);
  }
  free(record->words);
  free(record->name);
  free(record);
}

void read_template(FILE * f, catarray_t * cats, int reuse) {
  char * line = NULL;
  size_t size_ = 0;
  category_t * record = malloc(sizeof(*record));
  record->n_words = 0;
  record->name = strdup("USED");
  record->words = NULL;
  while (getline(&line, &size_, f) >= 0) {
    parse_template(line, cats, record, reuse);
  }
  free(line);
  freeRecord(record);
}

//step2
char * parse_input_inst(char * line) {
  char * l_end = strchr(line, '\n');
  *l_end = '\0';
  char * w_start = strchr(line, ':');
  if (w_start != NULL) {
    w_start += 1;
    char * instance = strdup(w_start);
    return instance;
  }
  else {
    fprintf(stderr, "NO ':' in the line %s. \n", line);
    exit(EXIT_FAILURE);
  }
}

char * parse_input_cat(char * line) {
  char * cat_end = strchr(line, ':');
  if (cat_end != NULL) {
    *cat_end = '\0';
    char * cat_ins = strdup(line);
    return cat_ins;
  }
  else {
    fprintf(stderr, "There is no ':' in line: %s. \n", line);
    exit(EXIT_FAILURE);
  }
}

int contain_cat(catarray_t * cats, char * cat) {
  size_t idx = 0;
  while (idx < cats->n) {
    if (strcmp(cat, cats->arr[idx].name) == 0)
      return idx;
    idx++;
  }
  return -1;
}

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
    if (-1 == contain_cat(cats, cat)) {  //new type
      cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
      cats->arr[cats->n].name = cat;
      cats->arr[cats->n].words = malloc(sizeof(*cats->arr[cats->n].words));
      cats->arr[cats->n].words[0] = inst;
      cats->arr[cats->n].n_words = 1;
      cats->n++;
      //free(cat);
    }
    else {
      free(cat);
      //int idx = contain_cat(cats, cat);
      cats->arr[idx].n_words++;
      cats->arr[idx].words = realloc(
          cats->arr[idx].words, cats->arr[idx].n_words * sizeof(*cats->arr[idx].words));
      cats->arr[idx].words[cats->arr[idx].n_words - 1] = inst;
    }
  }
  free(line);
  return cats;
}

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
const char * My_Choose_Word(char * cat, catarray_t * cats, category_t * record) {
  if (cats == NULL) {
    //const char * ans =
    return chooseWord(cat, NULL);
    //return ans;
  }
  else {
    int tmp_num = atoi(cat);
    int idx = contain_cat(cats, cat);
    if (tmp_num >= 1) {
      if ((size_t)tmp_num > record->n_words) {
        fprintf(stderr, "Do not have enough words. \n");
        exit(EXIT_FAILURE);
      }
      else {
        //const char * ans =
        return record->words[record->n_words - tmp_num];
        //return ans;
      }
    }
    else if (idx != -1 && cats->arr[idx].n_words) {
      //const char * ans =
      return chooseWord(cat, cats);
      //return ans;
    }
    else {
      fprintf(stderr, "Error from: doesn't contain enough words in category\n");
      exit(EXIT_FAILURE);
    }
  }
}

//step4
void update_cats(catarray_t * cats, char * cat, const char * word) {
  //printf("check");
  int idx = contain_cat(cats, cat);
  size_t i = 0;
  size_t w_idx;
  while (i < cats->arr[idx].n_words) {
    if (strcmp(word, cats->arr[idx].words[i]) == 0)
      w_idx = i;
    i++;
  }

  //update cats
  free(cats->arr[idx].words[w_idx]);
  cats->arr[idx].n_words--;

  char ** t = malloc(sizeof(*t) * cats->arr[idx].n_words);
  size_t m = 0;
  size_t n = 0;
  size_t k = 0;
  while (m < cats->arr[idx].n_words + 1) {
    if (w_idx != m) {
      t[n] = cats->arr[idx].words[m];
      n++;
    }
    m++;
  }
  free(cats->arr[idx].words);
  cats->arr[idx].words = malloc(sizeof(*cats->arr[idx].words) * cats->arr[idx].n_words);
  while (k < cats->arr[idx].n_words) {
    cats->arr[idx].words[k] = t[k];
    k++;
  }
  free(t);
}