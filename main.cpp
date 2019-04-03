#include <iostream>
#include "uint256.h"
#include "tests.h"
using namespace std;

void print_test(uint256 test[]) {
  for(int i = 0; i < 16; i++) {
    cout<<"Fp(";
    print(test[i]);
    cout<<")\n";
  }
}

uint32 bitreverse(uint32 n, uint32 bits) {
  uint32 r = 0;
  for(int i = 0; i < bits; i++) {
    r = (r << 1) | (n & 1);
    n >>= 1;
  }
  return r;
}

void swap(uint256 &a, uint256 &b) {
  uint256 tmp = a;
  a = b;
  b = tmp;
}

void FFT(uint256 *elems, uint32 n, uint32 lg, uint256 omega) {
  for(uint32 k = 0; k < n; k++) {
    uint32 rk = bitreverse(k, lg);
    if(k < rk)
      swap(elems[k], elems[rk]);
  }
}

int main() {
  print_test(test1);
  FFT(test1, 16, 4, ONE);
  cout<<"===\n";
  print_test(test1);
}
