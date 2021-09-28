#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering of the strings
//in qsort. You do not need to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void operations(FILE * f) {
  char ** array_ = NULL;
  char * string_line = NULL;
  size_t size_ = 0;
  size_t len = 0;
  while (getline(&string_line, &len, f) >= 0) {
    array_ = realloc(array_, (size_ + 1) * sizeof(*array_));
    array_[size_] = string_line;
    string_line = NULL;
    size_++;
  }
  free(string_line);
  sortData(array_, size_);

  for (size_t i = 0; i < size_; i++) {
    printf("%s", array_[i]);
    free(array_[i]);
  }
  free(array_);
}

int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  if (argc != 1) {
    for (int i = 1; i < argc; i++) {
      FILE * f = fopen(argv[i], "r");
      if (f == NULL) {
        fprintf(stderr, "Cannot open the file!\n");
        exit(EXIT_FAILURE);
      }
      operations(f);
      if (fclose(f) != 0) {
        fprintf(stderr, "Cannot close the file!\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  else {
    operations(stdin);
  }
  return EXIT_SUCCESS;
}
