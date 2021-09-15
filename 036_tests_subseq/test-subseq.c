#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n);

void helper(int * array, size_t n, size_t m) {
  if (maxSeq(array, n) == m) {
    printf("the func pass the test case \n");
  }
  else {
    printf("the func fail the test case \n");
    exit(EXIT_FAILURE);
  }
}

int main() {
  int a1[10] = {1, 2, 1, 3, 5, 7, 2, 4, 6, 9};
  int a2[3] = {3, 2, 1};
  int a3[1] = {0};
  int a4[4] = {-1, -2, -3, -4};
  int a5[4] = {-4, -3, -2, -1};
  int a6[3] = {2, 2, 2};
  int a7[3] = {-1, 0, 1};
  int a8[3] = {1, 2, 3};
  helper(a1, 10, 4);
  helper(a2, 3, 1);
  helper(a3, 1, 1);
  helper(a4, 4, 1);
  helper(a5, 4, 4);
  helper(a6, 3, 1);
  helper(a7, 3, 3);
  helper(a8, 0, 0);
  return EXIT_SUCCESS;
}
