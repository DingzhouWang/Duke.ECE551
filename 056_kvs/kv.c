#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

kvarray_t * readKVs(const char * fname) {
  //WRITE ME
  FILE * f = fopen(fname, "r");
  if (f == NULL) {
    fprintf(stderr, "The file cannot open!");
    exit(EXIT_FAILURE);
  }
  kvarray_t * my_kv_pair = malloc(sizeof(*my_kv_pair));
  my_kv_pair->arr_len = 0;
  my_kv_pair->kv_pair_array = NULL;
  size_t count = 0;
  char * line = NULL;
  size_t size_ = 0;
  while (getline(&line, &size_, f) >= 0) {
    my_kv_pair->arr_len++;
    my_kv_pair->kv_pair_array =
        realloc(my_kv_pair->kv_pair_array,
                sizeof(*my_kv_pair->kv_pair_array) * my_kv_pair->arr_len);
    kvpair_t * kvp = malloc(sizeof(*kvp));

    //char* p1 = line;
    // int cnt_len1 = 0;

    //while(*p1!='='){
    //p1++;
    //cnt_len1++;
    // }

    kvp->key = line;
    char * tmp = strchr(kvp->key, '=');
    if (tmp != NULL) {
      *tmp = '\0';
    }
    kvp->value = tmp + 1;
    char * end = strchr(tmp + 1, '\n');
    if (end != NULL) {
      *end = '\0';
    }
    my_kv_pair->kv_pair_array[count] = kvp;
    count++;
    line = NULL;
  }
  free(line);
  if (fclose(f) != 0) {
    fprintf(stderr, "Cannot close the file!");
    exit(EXIT_FAILURE);
  }
  return my_kv_pair;
}

void freeKVs(kvarray_t * pairs) {
  //WRITE ME
  for (size_t i = 0; i < pairs->arr_len; i++) {
    free(pairs->kv_pair_array[i]->key);
    free(pairs->kv_pair_array[i]);
  }
  free(pairs->kv_pair_array);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (size_t i = 0; i < pairs->arr_len; i++) {
    printf("key = '%s', value = '%s'\n",
           pairs->kv_pair_array[i]->key,
           pairs->kv_pair_array[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  char * v = NULL;
  for (size_t i = 0; i < pairs->arr_len; i++) {
    if (strcmp(pairs->kv_pair_array[i]->key, key) == 0) {
      v = pairs->kv_pair_array[i]->value;
    }
  }
  return v;
}
