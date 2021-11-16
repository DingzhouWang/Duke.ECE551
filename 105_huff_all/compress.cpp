#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>

#include "node.h"
#include "readFreq.h"

void writeHeader(BitFileWriter * bfw, const std::map<unsigned, BitString> & theMap) {
  for (int i = 0; i < 257; i++) {
    std::map<unsigned, BitString>::const_iterator it = theMap.find(i);
    if (it != theMap.end()) {
      bfw->writeByte(it->second.size());
      bfw->writeBitString(it->second);
    }
    else {
      bfw->writeByte(0);
    }
  }
}

void writeCompressedOutput(const char * inFile,
                           const char * outFile,
                           const std::map<unsigned, BitString> & theMap) {
  BitFileWriter bfw(outFile);
  writeHeader(&bfw, theMap);

  //WRITE YOUR CODE HERE!
  //open the input file for reading
  std::ifstream f;
  f.open(inFile);
  if (f.fail()) {
    std::cerr << "Cannot open file " << std::endl;
    exit(EXIT_FAILURE);
  }
  //You need to read the input file, lookup the characters in the map,
  //and write the proper bit string with the BitFileWriter
  int ch;
  while ((ch = f.get()) != EOF) {
    if (theMap.find(ch) == theMap.end()) {
      std::cerr << "Map doesn't contain character with int " << std::endl;
      exit(EXIT_FAILURE);
    }
    bfw.writeBitString(theMap.find(ch)->second);
  }
  //dont forget to lookup 256 for the EOF marker, and write it out.
  bfw.writeBitString(theMap.find(256)->second);
  //BitFileWriter will close the output file in its destructor
  //but you probably need to close your input file.
  f.close();
}

int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: compress input output\n");
    return EXIT_FAILURE;
  }
  //WRITE YOUR CODE HERE
  //Implement main
  //hint 1: most of the work is already done.
  //hint 2: you can look at the main from the previous tester for 90% of this
  uint64_t * c = readFrequencies(argv[1]);
  assert(c != NULL);
  Node * root = buildTree(c);
  delete[] c;
  std::map<unsigned, BitString> m_;
  BitString empty;
  root->buildMap(empty, m_);
  writeCompressedOutput(argv[1], argv[2], m_);
  delete root;
  return EXIT_SUCCESS;
}
