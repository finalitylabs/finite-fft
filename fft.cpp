#include <iostream>
#include "uint256.h"
#include "tests.h"
using namespace std;

void print(uint256 v) {
  printf("0x%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx",
    v.val[7],v.val[6],v.val[5],v.val[4],v.val[3],v.val[2],v.val[1],v.val[0]);
}

void print_test(uint256 test[]) {
  for(int i = 0; i < 16; i++) {
    cout<<"Fp(";
    print(test[i]);
    cout<<")\n";
  }
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

#define ONEE ((uint256){{0x00000001,0x00000000,0x00000000,0x00000000, 0x00000000,0x00000000,0x00000000,0x00000000}})
#define R2 ((uint256){{0xf3f29c6d,0xc999e990,0x87925c23,0x2b6cedcb, 0x7254398f,0x05d31496,0x9f59ff11,0x0748d9d9}})

int main() {
  for(int i = 0; i < 16; i++) test1_elements[i] = mulmod(test1_elements[i], R2);
  test1_omega = mulmod(test1_omega, R2);
  FFT(test1_elements, 16, 4, test1_omega);
  for(int i = 0; i < 16; i++) test1_elements[i] = mulmod(test1_elements[i], ONEE);
  print_test(test1_elements);
  cout<<"===\n";
  for(int i = 0; i < 16; i++) test2_elements[i] = mulmod(test2_elements[i], R2);
  test2_omega = mulmod(test2_omega, R2);
  FFT(test2_elements, 16, 4, test2_omega);
  for(int i = 0; i < 16; i++) test2_elements[i] = mulmod(test2_elements[i], ONEE);
  print_test(test2_elements);
}
