#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  //check arguments
  if (argc != 2) {
    fprintf(stderr, "Error input: please use ./story-step2 wordFile. \n");
    exit(EXIT_FAILURE);
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open the file. \n");
    exit(EXIT_FAILURE);
  }
  //read words.txt and get category and instances
  catarray_t * cat_arr = parse_cat_file(f);
  //print cat_arr
  printWords(cat_arr);
  //free catarray_t*
  freeCat(cat_arr);
  if (fclose(f) != 0) {
    fprintf(stderr, "Cannot close the file. \n");
    exit(EXIT_FAILURE);
  }
}
