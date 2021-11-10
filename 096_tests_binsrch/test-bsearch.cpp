#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "function.h"

class CountedIntFn : public Function<int, int> {
 protected:
  unsigned remaining;
  Function<int, int> * f;
  const char * mesg;

 public:
  CountedIntFn(unsigned n, Function<int, int> * fn, const char * m) :
      remaining(n),
      f(fn),
      mesg(m) {}
  virtual int invoke(int arg) {
    if (remaining == 0) {
      fprintf(stderr, "Too many function invocations in %s \n", mesg);
      exit(EXIT_FAILURE);
    }
    remaining--;
    return f->invoke(arg);
  }
};

int binarySearchForZero(Function<int, int> * f, int low, int high);

class SinFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return 10000000 * (sin(arg / 100000.0) - 0.5); }
};

class nFunction : public Function<int, int> {
 public:
  virtual int invoke(int arg) { return arg - 10000.5; }
};

void check(Function<int, int> * f,
           int low,
           int high,
           int expected_ans,
           const char * mesg) {
  int n = log2(high - low) + 1;
  if (low >= high)
    n = 1;
  CountedIntFn * f_ = new CountedIntFn(n, f, mesg);
  int ans = binarySearchForZero(f_, low, high);
  if (expected_ans != ans) {
    std::cout << mesg << std::endl;
    exit(EXIT_FAILURE);
  }
  delete f_;
}

int main(void) {
  Function<int, int> * f1 = new SinFunction();
  check(f1, 0, 150000, 52359, "SinFunction");
  check(f1, 0, 52359, 52358, "SinFunction");
  check(f1, 52359, 150000, 52359, "SinFunction");
  delete f1;

  Function<int, int> * f2 = new nFunction();
  check(f2, -200000, 150000, 10000, "nFunction");

  delete f2;

  return EXIT_SUCCESS;
}
