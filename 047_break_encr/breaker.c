#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int helper(FILE * f, int * array, int n) {
  int c;
  int idx=0;
  while ((c = fgetc(f)) != EOF) {
    if (isalpha(c)) {
      c = tolower(c);
      array[c - 97]++;
    }
  }
  for (int i = 1; i < n; i++) {
    if (array[i] > array[idx]) {
      idx = i;
    }
  }
  return idx + 97;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: breaker inputFileName\n");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  int array[26] = {0};
  int ch_e = helper(f, array, 26);
  int key;
  if (ch_e >= 101) {
    key = ch_e - 101;
  }
  else {
    key = 26 + ch_e - 101;
  }
  printf("%d\n", key);
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
