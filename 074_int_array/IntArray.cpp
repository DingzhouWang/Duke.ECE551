#include "IntArray.h"

#include <assert.h>

#include <ostream>

IntArray::IntArray() : data(NULL), numElements(0) {
}
IntArray::IntArray(int n) : data(new int[n]), numElements(n) {
}

IntArray::IntArray(const IntArray & rhs) {
  this->numElements = rhs.numElements;
  this->data = new int[this->numElements];
  for (int i = 0; i < this->numElements; i++) {
    this->data[i] = rhs.data[i];
  }
}
IntArray::~IntArray() {
  delete[] data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
  if (*this == rhs)
    return *this;
  delete[] data;
  this->numElements = rhs.numElements;
  this->data = new int[this->numElements];
  for (int i = 0; i < numElements; i++) {
    this->data[i] = rhs.data[i];
  }
  return *this;
}
const int & IntArray::operator[](int index) const {
  assert(0 <= index && index < numElements);
  return data[index];
}
int & IntArray::operator[](int index) {
  assert(0 <= index && index < numElements);
  return data[index];
}

int IntArray::size() const {
  return numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
  if (this->numElements != rhs.numElements) {
    return false;
  }
  for (int i = 0; i < this->numElements; i++) {
    if (this->data[i] != rhs.data[i])
      return false;
  }
  return true;
}

bool IntArray::operator!=(const IntArray & rhs) const {
  return !(*this == rhs);
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
  s << '{';
  for (int i = 0; i < rhs.size() - 1; i++) {
    s << rhs[i] << ", ";
  }
  s << rhs[rhs.size() - 1];
  s << '}';
  return s;
}
