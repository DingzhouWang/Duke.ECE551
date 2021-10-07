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
    //return NULL;
  }
  kvarray_t * my_kv_pair = malloc(sizeof(*my_kv_pair));
  if (my_kv_pair == NULL) {
    fprintf(stderr, "Do not have enough memory");
    exit(EXIT_FAILURE);
  }
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

    if (my_kv_pair->kv_pair_array == NULL) {
      fprintf(stderr, "Malloc Failed!");
      //exit(EXIT_FAILURE);
      return NULL;
    }
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
    else {
      exit(EXIT_FAILURE);
    }
    kvp->value = tmp + 1;
    char * end = strchr(kvp->value, '\n');
    if (end != NULL) {
      *end = '\0';
    }
    else {
      exit(EXIT_FAILURE);
    }
    my_kv_pair->kv_pair_array[count] = kvp;
    count++;
    line = NULL;
  }
  free(line);
  if (fclose(f)) {
    fprintf(stderr, "Cannot close the file!");
    return NULL;
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
    printf("key = '%s' value = '%s'\n",
           pairs->kv_pair_array[i]->key,
           pairs->kv_pair_array[i]->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  char * res = NULL;
  for (size_t i = 0; i < pairs->arr_len; i++) {
    if (strcmp(pairs->kv_pair_array[i]->key, key) == 0) {
      res = pairs->kv_pair_array[i]->value;
    }
  }
  return res;
}
