#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * array, size_t n) {
  size_t ans = 0;
  size_t cnt = 1;
  if (n == 0)
    return ans;
  for (size_t i = 1; i < n; i++) {
    if (array[i] > array[i - 1]) {
      cnt++;
    }
    else {
      if (ans < cnt)
        ans = cnt;
      cnt = 1;
    }
  }
  if (ans < cnt)
    return cnt;
  return ans;
}
