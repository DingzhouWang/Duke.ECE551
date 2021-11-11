#include <cstdio>

#include "function.h"

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  if (high - low <= 1)
    return low;
  int mid = low + (high - low) / 2;
  int num = f->invoke(mid);
  if (num == 0)
    return mid;
  else if (num < 0)
    return binarySearchForZero(f, mid, high);
  else
    return binarySearchForZero(f, low, mid);
}
