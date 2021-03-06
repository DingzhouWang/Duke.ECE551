#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  //check number of arguments
  if (argc != 3) {
    fprintf(stderr, "Error input: please use ./story-step3 wordfile templtefile. \n");
    exit(EXIT_FAILURE);
  }
  FILE * f_word = fopen(argv[1], "r");
  catarray_t * cats;
  //check open word file
  if (f_word != NULL) {
    //parse category file
    cats = parse_cat_file(f_word);
  }
  else {
    fprintf(stderr, "Cannot open %s. \n", argv[1]);
    exit(EXIT_FAILURE);
  }
  //check close word file
  if (fclose(f_word) != 0) {
    fprintf(stderr, "Can not close %s. \n", argv[1]);
    exit(EXIT_FAILURE);
  }
  FILE * f_temp = fopen(argv[2], "r");
  //check open template file
  if (f_temp != NULL) {
    //do something
    // parse template ans print story with proper words
    read_template(f_temp, cats, 1);
  }
  else {
    fprintf(stderr, "Can not open %s. \n", argv[2]);
    exit(EXIT_FAILURE);
  }
  //check close template file
  if (fclose(f_temp) != 0) {
    fprintf(stderr, "Can not close %s. \n", argv[2]);
    exit(EXIT_FAILURE);
  }
  //free
  freeCat(cats);
}
