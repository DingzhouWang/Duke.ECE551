#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counts.h"
#include "kv.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  //WRITE ME
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "Cannot open the file!\n");
    return NULL;
  }
  counts_t * ans = createCounts();
  char * line = NULL;
  size_t len = 0;
  while (getline(&line, &len, f) >= 0) {
    char * tmp = strchr(line, '\n');
    if (tmp != NULL) {
      *tmp = '\0';
    }
    char * value = lookupValue(kvPairs, line);
    addCount(ans, value);
  }
  free(line);
  if (fclose(f)) {
    fprintf(stderr, "Cannot close the file!");
    return NULL;
  }
  return ans;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
  if (argc < 2) {
    fprintf(stderr, "lack of arguments!\n");
    exit(EXIT_FAILURE);
  }
  //read the key/value pairs from the file named by argv[1] (call the result kv)
  kvarray_t * kv = readKVs(argv[1]);
  //count from 2 to argc (call the number you count i)
  for (int i = 2; i < argc; i++) {
    //count the values that appear in the file named by argv[i], using kv as the key/value pair
    //   (call this result c)
    counts_t * c = countFile(argv[i], kv);
    //compute the output file name from argv[i] (call this outName)
    char * outName = computeOutputFileName(argv[i]);
    //open the file named by outName (call that f)
    FILE * f = fopen(outName, "w");
    if (f == NULL) {
      fprintf(stderr, "Could not open the newfile\n");
      exit(EXIT_FAILURE);
    }
    //print the counts from c into the FILE f
    printCounts(c, f);
    //close f
    if (fclose(f)) {
      fprintf(stderr, "Cannot close the file!");
      exit(EXIT_FAILURE);
    }

    //free the memory for outName and c
    free(outName);
    //free the memory for kv
    freeCounts(c);
  }
  freeKVs(kv);
  return EXIT_SUCCESS;
}
