#include <iostream>

#define LIMBS 8
#define P ((field){{0x00000001,0xffffffff,0xfffe5bfe,0x53bda402,0x09a1d805,0x3339d808,0x299d7d48,0x73eda753}})
#define ONE ((field){{0xfffffffe,0x00000001,0x00034802,0x5884b7fa,0xecbc4ff5,0x998c4fef,0xacc5056f,0x1824b159}})
#define ZERO ((field){{0, 0, 0, 0, 0, 0, 0, 0}})
#define INV ((uint32)4294967295)
#include "field.h"

#include "tests.h"
using namespace std;

void print(field v) {
  printf("0x%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx",
    v.val[7],v.val[6],v.val[5],v.val[4],v.val[3],v.val[2],v.val[1],v.val[0]);
}

void print_test(field test[]) {
  for(int i = 0; i < 16; i++) {
    cout<<"Fp(";
    print(test[i]);
    cout<<")\n";
  }
}

void swap(field *a, field *b) {
  field tmp = *a;
  *a = *b;
  *b = tmp;
}

void FFT(field *elems, uint32 n, uint32 lg, field omega) {

  for(uint32 k = 0; k < n; k++) {
    uint32 rk = bitreverse(k, lg);
    if(k < rk)
      swap(&elems[k], &elems[rk]);
  }

  uint32 m = 1;
  for(int i = 0; i < lg; i++) {
    field w_m = powmod(omega, n / (2*m));
    uint32 k = 0;
    while(k < n) {
      field w = ONE;
      for(int j = 0; j < m; j++) {
        field t = elems[k+j+m];
        t = mulmod(t, w);
        field tmp = elems[k+j];
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

#define ONEE ((field){{0x00000001,0x00000000,0x00000000,0x00000000, 0x00000000,0x00000000,0x00000000,0x00000000}})
#define R2 ((field){{0xf3f29c6d,0xc999e990,0x87925c23,0x2b6cedcb, 0x7254398f,0x05d31496,0x9f59ff11,0x0748d9d9}})

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
