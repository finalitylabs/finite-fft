#include <iostream>
using namespace std;

#define LIMBS 12
#define P ((field){{0xffffaaab,0xb9feffff,0xb153ffff,0x1eabfffe,0xf6b0f624,0x6730d2a0,0xf38512bf,0x64774b84,0x434bacd7,0x4b1ba7b6,0x397fe69a,0x1a0111ea}})
#define ONE ((field){{0x0002fffd,0x76090000,0xc40c0002,0xebf4000b,0x53c758ba,0x5f489857,0x70525745,0x77ce5853,0xa256ec6d,0x5c071a97,0xfa80e493,0x15f65ec3}})
#define ZERO ((field){{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})
#define INV ((uint32)4294770685)
#include "field.h"
#include "field2.h"
#include "ec.h"
#include "ec2.h"

#define ONEE ((field){{1,0,0,0,0,0,0,0,0,0,0,0}})
#define R2 ((field){{0x1c341746,0xf4df1f34,0x09d104f1,0x0a76e6a6,0x4c95b6d5,0x8de5476c,0x939d83c0,0x67eb88a9,0xb519952d,0x9a793e85,0x92cae3aa,0x11988fe5}})

void print(field v) {
  printf("0x%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx",
    v.val[11],v.val[10],v.val[9],v.val[8],v.val[7],v.val[6],v.val[5],v.val[4],v.val[3],v.val[2],v.val[1],v.val[0]);
}

bool ec_working() {
  field x = {{0x94085a32,0xc33e270a,0x9254b34f,0xc5345b77,0xcb0463f9,0x643998e2,0x45efaca7,0x63fb918d,0xa93d6136,0x9a7cac11,0x3132560b,0x06a7ab96}};
  field y = {{0xe0a1219d,0xa106b24f,0x7871ed91,0x94a22b97,0xfef164a7,0x5504a2ac,0x3d452ce8,0x4fbec8e6,0x3828c2c9,0x4dbe7483,0x5ea41f0b,0x1078c2b6}};
  x = mulmod(x, R2); y = mulmod(y, R2);

  projective p = {x, y, ONE};
  p = ec_double(p);
  p = ec_add(p, (affine){x, y, false});

  // Convert Projective to Affine
  field zinv = {{0x57f61f83,0xcd26d647,0xfb3c196d,0x83a8efbc,0x2470bd22,0x2768a181,0x76975814,0xdf3c0fa1,0x1898656a,0x30f3c6a3,0xa09c43b2,0x151e6cc4}};
  zinv = mulmod(zinv, R2);
  field zinv2 = mulmod(zinv, zinv);
  field zinv3 = mulmod(zinv2, zinv);
  p.x = mulmod(p.x, zinv2);
  p.y = mulmod(p.y, zinv3);
  p.z = mulmod(p.z, zinv);

  p.x = mulmod(p.x, ONEE); p.y = mulmod(p.y, ONEE); p.z = mulmod(p.z, ONEE);
  
  field x2 = {{0x978780a8,0x61ed9247,0x80aa953c,0x8f2fdb95,0x8fd0e44c,0xc8cfc557,0x841d8191,0x5f00ad77,0x38628008,0xf09c79e7,0x2ad4a406,0x08bbfe5f}};
  field y2 = {{0xa61acf46,0x72a8ee56,0xe8a0ded6,0x94ae2e5d,0x55276aaf,0x7730af28,0x7aae47cd,0xb384cc14,0x8d8adaab,0x560061ca,0x9b19cbee,0x0a810ac2}};
  return eq(p.x, x2) && eq(p.y, y2);
}

bool ec2_working() {
  field2 x = {{{0xd96aad8d,0xbec51448,0xd82541ad,0xdb75f1c7,0x5bb6efef,0xd5b6b506,0xe124f4cb,0x190e86ad,0x1a1e679b,0x875dba26,0x1eb7053c,0x0c857ffb}},
              {{0x564d2cea,0xfdec778f,0xdd1fe552,0x1daa6c4f,0x940312ed,0xa87bcf0c,0x388e1ad9,0x817416dc,0xdf070060,0x14116684,0xcc1abc75,0x0335cfcb}}};
  field2 y = {{{0x56175926,0x1eb88aa7,0xb8ba0577,0x67bf4dca,0x636a5860,0x8f9befee,0x7de3f4b5,0xc8864ba9,0xf8ab2ba0,0x1c7bb33a,0x6be64e95,0x0385099c}},
              {{0x32edd56e,0xd2c8e95a,0xfe31cc54,0xbac81359,0xe77cc4ab,0xa8e00ceb,0x80755ef2,0xd43eacf5,0xbe176df5,0xf8a9dc09,0xd8f5fa9c,0x02fa830f}}};
  x.c0 = mulmod(x.c0, R2); x.c1 = mulmod(x.c1, R2);
  y.c0 = mulmod(y.c0, R2); y.c1 = mulmod(y.c1, R2);

  projective2 p = {x, y, ONE2};
  p = ec2_double(p);
  p = ec2_add(p, (affine2){x,y,false});

  // Convert Projective to Affine
  field2 zinv = {{{0x59ee68c6,0x0c1e864e,0x05531edc,0x81ada41d,0xe6a8bf59,0x171be1a4,0x61e57de9,0x889ac377,0xc969721e,0x289a5334,0x262579e8,0x154be248}},
                 {{0x399ec530,0xb71760e1,0x078eda19,0xd83f2d37,0x7c7c2bc0,0xf9158157,0xfd53567c,0xcc357673,0xc660bc14,0xfe748d54,0x3d5f3374,0x0f68e6bf}}};
  zinv.c0 = mulmod(zinv.c0, R2); zinv.c1 = mulmod(zinv.c1, R2);
  field2 zinv2 = mulmod2(zinv, zinv);
  field2 zinv3 = mulmod2(zinv2, zinv);
  p.x = mulmod2(p.x, zinv2);
  p.y = mulmod2(p.y, zinv3);
  p.z = mulmod2(p.z, zinv);

  p.x.c0 = mulmod(p.x.c0, ONEE); p.x.c1 = mulmod(p.x.c1, ONEE);
  p.y.c0 = mulmod(p.y.c0, ONEE); p.y.c1 = mulmod(p.y.c1, ONEE);
  p.z.c0 = mulmod(p.z.c0, ONEE); p.z.c1 = mulmod(p.z.c1, ONEE);

  field2 x2 = {{{0x78ef93fa,0x34b792eb,0x670a0530,0xb68d5f3b,0xa24f299d,0xbe08d8bb,0xe30b85bb,0xef3250d8,0x7b4dda30,0xeae78fbd,0x1f5d81a5,0x0fbb908d}},
               {{0x5748ddb3,0xe4229a81,0xbc8265e7,0x1321f221,0x040b63e8,0x82cd621d,0x70693c10,0xaa56dcee,0x86f73982,0xa1e1ea62,0x5dbc407b,0x16373b5e}}};
  field2 y2 = {{{0x01241c58,0x00b50d5d,0x27d10380,0x5ad20036,0x33b8a578,0x9b9ede8d,0x8fe92892,0x3246d79a,0x679b4405,0x7a56ec83,0xfc12f53c,0x01f57dd7}},
               {{0x41ab3970,0x996dc8ca,0x6eeaf21f,0xb14a887e,0x378b1589,0x2f4fb9ed,0xeab72157,0xaeeb0167,0xacf13cf9,0x0014a2f9,0x85a64eab,0x0620b522}}};

  return eq2(p.x, x2) && eq2(p.y, y2);
}

int main() {
  cout<<ec_working()<<endl;
  cout<<ec2_working()<<endl;
}
