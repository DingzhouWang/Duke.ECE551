#include <stdio.h>
#include <stdlib.h>

#include "rand_story.h"

int main(int argc, char ** argv) {
  //check number of arguments
  if (argc != 2) {
    fprintf(stderr, "Error input: Please use ./story-step1.c storyFlie. \n");
    exit(EXIT_FAILURE);
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open the file! \n");
    exit(EXIT_FAILURE);
  }
  //read and parse the tamplate.
  read_template(f, NULL, 1);

  if (fclose(f) != 0) {
    fprintf(stderr, "Cannot close the file! \n");
    exit(EXIT_FAILURE);
  }
}
