#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void rotate_matrix(FILE * file) {
  size_t row = 0;
  size_t col = 0;
  int read_tmp;
  char matrix[10][10] = {
      "0000000000",
      "0000000000",
      "0000000000",
      "0000000000",
      "0000000000",
      "0000000000",
      "0000000000",
      "0000000000",
      "0000000000",
      "0000000000",
  };
  //init the matrix
  while ((read_tmp = fgetc(file)) != EOF) {
    if (read_tmp != '\n') {
      if (row < 0 || row >= 10 || col < 0 || col >= 10) {
        fprintf(stderr, "Out of index! The matrix is invalid. \n");
        exit(EXIT_FAILURE);
      }
      else {
        if (0 < read_tmp && read_tmp <= 255) {
          matrix[row][col] = read_tmp;
          col++;
        }
        else {
          fprintf(stderr, "Input Invalid character. \n");
          exit(EXIT_FAILURE);
        }
      }
    }
    else {
      if (col != 10) {
        fprintf(stderr, "Less col, Invalid matrix. \n");
        exit(EXIT_FAILURE);
      }
      else {
        col = 0;
        row++;
      }
    }
  }

  //check row
  if (row != 10) {
    fprintf(stderr, "Less row. Invalid matrix. \n");
    exit(EXIT_FAILURE);
  }

  //  rotate
  for (int m = 0; m < 10; m++) {
    for (int n = 9; n >= 0; n--) {
      fprintf(stdout, "%c", matrix[n][m]);
    }
    fprintf(stdout, "\n");
  }
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage:rotateMatrix inputFileName\n");
    return EXIT_FAILURE;
  }
  FILE * file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  rotate_matrix(file);
  if (fclose(file) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
