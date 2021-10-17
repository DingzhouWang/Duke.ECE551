#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rand_story.h"
int main(int argc, char ** argv) {
  if (argc != 3 && argc != 4) {
    fprintf(stderr, "Useage: ./story_step4 (-n) wordfile templatefile.\n");
    exit(EXIT_FAILURE);
  }
  if (argc == 3 && strcmp(argv[1], "-n") != 0) {
    FILE * f_w = fopen(argv[1], "r");
    if (f_w == NULL) {
      fprintf(stderr, "Cannot open wordfile.\n");
      exit(EXIT_FAILURE);
    }
    catarray_t * cats = parse_cat_file(f_w);
    FILE * f_t = fopen(argv[2], "r");
    if (f_t == NULL) {
      fprintf(stderr, "Cannot open templatefile.\n");
      exit(EXIT_FAILURE);
    }
    read_template(f_t, cats, 1);

    if (fclose(f_w) != 0) {
      fprintf(stderr, "Cannot close wordfile.\n");
      exit(EXIT_FAILURE);
    }
    if (fclose(f_t) != 0) {
      fprintf(stderr, "Can not close templatefile.\n");
      exit(EXIT_FAILURE);
    }
    freeCat(cats);
  }
  else if (argc == 4 && strcmp(argv[1], "-n") == 0) {
    //printf("11 \n");
    FILE * f_w = fopen(argv[2], "r");
    if (f_w == NULL) {
      fprintf(stderr, "Cannot open wordfile.\n");
      exit(EXIT_FAILURE);
    }
    //printf("1.5 \n");
    catarray_t * cats = parse_cat_file(f_w);
    FILE * f_t = fopen(argv[3], "r");
    if (f_t == NULL) {
      fprintf(stderr, "Cannot open templatefile.\n");
      exit(EXIT_FAILURE);
    }
    //printf("22 \n");
    read_template(f_t, cats, 0);

    if (fclose(f_w) != 0) {
      fprintf(stderr, "Cannot close wordfile.\n");
      exit(EXIT_FAILURE);
    }
    if (fclose(f_t) != 0) {
      fprintf(stderr, "Can not close templatefile.\n");
      exit(EXIT_FAILURE);
    }
    freeCat(cats);
  }
  else {
    fprintf(stderr, "Other errors! \n");
    exit(EXIT_FAILURE);
  }
}