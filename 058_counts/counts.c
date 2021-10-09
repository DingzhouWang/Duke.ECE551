#include "counts.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
counts_t * createCounts(void) {
  //WRITE ME
  counts_t * myCount = malloc(sizeof(*myCount));
  if (myCount == NULL) {
    fprintf(stderr, "Error malloc for myCount!");
    return NULL;
  }
  myCount->array = NULL;
  myCount->cnt_unknown = 0;
  myCount->size_ = 0;
  return myCount;
}
void addCount(counts_t * c, const char * name) {
  //WRITE ME
  //bool add_new = false;
  // flag = 0;
  if (name == NULL) {
    c->cnt_unknown += 1;
  }
  else {
    bool flag = false;
    for (int i = 0; i < c->size_; i++) {
      if (strcmp(name, c->array[i]->string) == 0) {
        c->array[i]->times += 1;
        flag = true;
        return;
      }
    }
    if (!flag) {
      c->size_++;
      c->array = realloc(c->array, c->size_ * sizeof(*c->array));
      one_count_t * new_arr = malloc(sizeof(*new_arr));
      c->array[c->size_ - 1] = new_arr;
      c->array[c->size_ - 1]->string = name;
      c->array[c->size_ - 1]->times = 1;
    }
  }
}
void printCounts(counts_t * c, FILE * outFile) {
  //WRITE ME
  for (int i = 0; i < c->size_; i++) {
    fprintf(outFile, "%s: %d\n", c->array[i]->string, c->array[i]->times);
  }
  if (c->cnt_unknown > 0) {
    fprintf(outFile, "<unknown>: %d\n", c->cnt_unknown);
  }
}

void freeCounts(counts_t * c) {
  //WRITE ME
  for (int i = 0; i < c->size_; i++) {
    free(c->array[i]);
  }
  free(c->array);
  free(c);
}
