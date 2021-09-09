#include <stdio.h>
#include <stdlib.h>

unsigned helper_(unsigned x, unsigned y, unsigned ans) {
  if (y == 0)
    return ans;
  if (x == 0)
    return 0;
  return helper_(x, y - 1, ans * x);
}

unsigned power(unsigned x, unsigned y) {
  return helper_(x, y, 1);
}
