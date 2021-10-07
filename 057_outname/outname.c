#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * computeOutputFileName(const char * inputName) {
  //WRITE ME
  char * outname = malloc((strlen(inputName) + 8) * sizeof(*outname));
  memcpy(outname, inputName, strlen(inputName));
  memcpy(outname + strlen(inputName), ".counts", 8);
  //printf("%s", outname);
  return outname;
}
