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

void swap(uint256 *a, uint256 *b) {
  uint256 tmp = *a;
  *a = *b;
  *b = tmp;
}

void FFT(uint256 *elems, uint32 n, uint32 lg, uint256 omega) {

  for(uint32 k = 0; k < n; k++) {
    uint32 rk = bitreverse(k, lg);
    if(k < rk)
      swap(&elems[k], &elems[rk]);
  }

  uint32 m = 1;
  for(int i = 0; i < lg; i++) {
    uint256 w_m = powmod(omega, n / (2*m));
    uint32 k = 0;
    while(k < n) {
      uint256 w = ONE;
      for(int j = 0; j < m; j++) {
        uint256 t = elems[k+j+m];
        t = mulmod(t, w);
        uint256 tmp = elems[k+j];
        tmp = submod(tmp, t);
        elems[k+j+m] = tmp;
        elems[k+j] = addmod(elems[k+j], t);
        w = mulmod(w, w_m);
      }
      k += 2*m;
    }
    m *= 2;
  }
}

int main() {
  FFT(test1_elements, 16, 4, test1_omega);
  FFT(test2_elements, 16, 4, test2_omega);

  print_test(test1_elements);
  cout<<"===\n";
  print_test(test2_elements);
}
