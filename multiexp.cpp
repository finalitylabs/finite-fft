#include <iostream>

#define LIMBS 12
#define P ((field){{0xffffaaab,0xb9feffff,0xb153ffff,0x1eabfffe,0xf6b0f624,0x6730d2a0,0xf38512bf,0x64774b84,0x434bacd7,0x4b1ba7b6,0x397fe69a,0x1a0111ea}})
#define ONE ((field){{0x0002fffd,0x76090000,0xc40c0002,0xebf4000b,0x53c758ba,0x5f489857,0x70525745,0x77ce5853,0xa256ec6d,0x5c071a97,0xfa80e493,0x15f65ec3}})
#define ZERO ((field){{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})
#define INV ((uint32)4294770685)
#include "field.h"

#include "ec.h"
using namespace std;

void print(field v) {
  printf("0x%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx",
    v.val[11],v.val[10],v.val[9],v.val[8],v.val[7],v.val[6],v.val[5],v.val[4],v.val[3],v.val[2],v.val[1],v.val[0]);
}

#define ONEE ((field){{1,0,0,0,0,0,0,0,0,0,0,0}})
#define R2 ((field){{0x1c341746,0xf4df1f34,0x09d104f1,0x0a76e6a6,0x4c95b6d5,0x8de5476c,0x939d83c0,0x67eb88a9,0xb519952d,0x9a793e85,0x92cae3aa,0x11988fe5}})

int main() {
  field x = {{0x7f0c4b2c,0x0cc9d695,0x0f0fce9e,0xe5c67cd5,0xbef499ec,0x0dd1c0b8,0x41971e18,0x596a81ff,0xda80aa85,0x554f505e,0x2fee68df,0x024b06a4}};
  field y = {{0x8c2dce41,0x6cb0ed93,0xd3c25273,0xc3de3733,0x5912ea64,0x0e841707,0x8e713d34,0x72d73d89,0xddfcfbb7,0x6bc96aa1,0x90a45394,0x04365c83}};
  x = mulmod(x, R2); y = mulmod(y, R2);

  affine aff = {x, y, false};
  projective p = {x, y, ONE};
  p = doub(p);
  p = add(p, aff);

  x = mulmod(p.x, ONEE); y = mulmod(p.y, ONEE);
  print(x); cout<<endl;
  print(y); cout<<endl;
}
