#include "rand_story.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//step1
void parse_template(char * line) {
  size_t idx = 0, cat_len = 0;
  char * cat = NULL;
  bool blank = false;

  while (line[idx] != '\0') {
    if (line[idx] != '_' && !blank) {  //not in the blank
      printf("%c", line[idx]);
      idx += 1;
    }
    else if (line[idx] == '_' && !blank) {  //here is a new blank
      idx += 1;
      blank = true;
    }
    else if (line[idx] != '_' && blank) {  //in the blank
      //do something
      size_t start = idx;
      size_t tmp_i = 0;
      while (line[idx] != '\0' && line[idx] != '_') {
        cat_len++;
        idx++;
      }
      cat = realloc(cat, (cat_len + 1) * sizeof(*cat));
      while (start < idx) {
        cat[tmp_i] = line[start];
        tmp_i++;
        start++;
      }
      cat[cat_len] = '\0';
      //idx -= 1;
    }
    else if (line[idx] == '_' && blank) {  //end of blank
      //do something
      //printf("123 \n");
      const char * word = chooseWord(cat, NULL);
      cat_len = 0;
      blank = false;
      idx++;
      printf("%s", word);
    }
  }
  free(cat);
  if (blank == true) {  //no matching '_'
    fprintf(stderr, "Cannot match the '_': %s \n", line);
    exit(EXIT_FAILURE);
  }
}

void read_template(FILE * f) {
  char * line = NULL;
  size_t size_ = 0;
  while (getline(&line, &size_, f) >= 0) {
    parse_template(line);
  }
  free(line);
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
      int idx = contain_cat(cats, cat);
      cats->arr[idx].n_words++;
      cats->arr[idx].words = realloc(
          cats->arr[idx].words, cats->arr[idx].n_words * sizeof(*cats->arr[idx].words));
      cats->arr[idx].words[cats->arr[idx].n_words - 1] = inst;
    }
  }
  free(line);
  return cats;
}

//step3
