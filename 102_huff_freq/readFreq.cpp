#include "readFreq.h"

#include <stdio.h>

#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;
void printSym(std::ostream & s, unsigned sym) {
  if (sym > 256) {
    s << "INV";
  }
  else if (sym == 256) {
    s << "EOF";
  }
  else if (isprint(sym)) {
    char c = sym;
    s << "'" << c << "'";
  }
  else {
    std::streamsize w = s.width(3);
    s << std::hex << sym << std::dec;
    s.width(w);
  }
}
uint64_t * readFrequencies(const char * fname) {
  //WRITE ME!
  ifstream file;
  file.open(fname);
  if (!file.is_open()) {
    cerr << "cannot open the file: " << fname << endl;
    exit(EXIT_FAILURE);
  }
  uint64_t * count = new uint64_t[257]();
  int c;
  while ((c = file.get()) != EOF) {
    if (c > 255) {
      continue;
    }
    count[c]++;
  }
  count[256] = 1;
  file.close();
  return count;
}
