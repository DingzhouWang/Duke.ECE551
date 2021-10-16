#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Error input: please use ./story-step2 wordFile. \n");
    exit(EXIT_FAILURE);
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open the file. \n");
    exit(EXIT_FAILURE);
  }
  //do something
  catarray_t * cat_arr = parse_cat_file(f);
  printWords(cat_arr);
  freeCat(cat_arr);
  if (fclose(f) != 0) {
    fprintf(stderr, "Cannot close the file. \n");
    exit(EXIT_FAILURE);
  }
}
