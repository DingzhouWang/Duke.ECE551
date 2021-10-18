#include "IntMatrix.h"

IntMatrix::IntMatrix() : numRows(0), numColumns(0), rows(NULL) {
}
IntMatrix::IntMatrix(int r, int c) : numRows(r), numColumns(c) {
  rows = new IntArray *[numRows];
  for (int i = 0; i < numRows; i++) {
    rows[i] = new IntArray(numColumns);
  }
}
IntMatrix::IntMatrix(const IntMatrix & rhs) {
  this->numRows = rhs.numRows;
  this->numColumns = rhs.numColumns;
  this->rows = new IntArray *[numRows];
  for (int i = 0; i < numRows; i++) {
    rows[i] = new IntArray(numColumns);
    *rows[i] = *rhs.rows[i];
  }
}
IntMatrix::~IntMatrix() {
  for (int i = 0; i < numRows; i++) {
    delete rows[i];
  }
  delete[] rows;
}
IntMatrix & IntMatrix::operator=(const IntMatrix & rhs) {
  if (*this == rhs)
    return *this;
  for (int i = 0; i < numRows; i++) {
    delete rows[i];
  }
  delete[] rows;
  this->numRows = rhs.numRows;
  this->numColumns = rhs.numColumns;
  this->rows = new IntArray *[numRows];
  for (int i = 0; i < numRows; i++) {
    rows[i] = new IntArray(numColumns);
    *rows[i] = *rhs.rows[i];
  }
  return *this;
}
int IntMatrix::getRows() const {
  return numRows;
}
int IntMatrix::getColumns() const {
  return numColumns;
}
const IntArray & IntMatrix::operator[](int index) const {
  assert(0 <= index && index < numRows);
  return *rows[index];
}
IntArray & IntMatrix::operator[](int index) {
  assert(0 <= index && index < numRows);
  return *rows[index];
}

bool IntMatrix::operator==(const IntMatrix & rhs) const {
  if (this->numRows != rhs.numRows || this->numColumns != rhs.numColumns)
    return false;
  for (int i = 0; i < numRows; i++) {
    if (*rows[i] != *rhs.rows[i])
      return false;
  }
  return true;
}

IntMatrix IntMatrix::operator+(const IntMatrix & rhs) const {
  assert(this->numRows == rhs.numRows && this->numColumns == rhs.numColumns);
  IntMatrix ans(rhs.numRows, rhs.numColumns);
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      ans[i][j] = (*this)[i][j] + rhs[i][j];
    }
  }
  return ans;
}

std::ostream & operator<<(std::ostream & s, const IntMatrix & rhs) {
  if (rhs.getRows() == 0)
    s << "[]";
  else {
    s << '[';
    for (int i = 0; i < rhs.getRows(); i++) {
      s << rhs[i] << ",\n";
    }
    s << rhs[rhs.getRows() - 1];
    s << " ]";
  }
  return s;
}
