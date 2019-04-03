#ifndef UINT256_H
#define UINT256_H

typedef unsigned int uint32;
typedef unsigned long long uint64;

struct uint256 {
  uint32 val[8];
};

void print(uint256 v) {
  printf("0x%08llx%08llx%08llx%08llx%08llx%08llx%08llx%08llx",
    v.val[7],v.val[6],v.val[5],v.val[4],v.val[3],v.val[2],v.val[1],v.val[0]);
}


uint256 P({0x00000001,0xffffffff,0xfffe5bfe,0x53bda402,
            0x09a1d805,0x3339d808,0x299d7d48,0x73eda753});

uint256 ONE({0x00000001,0x00000000,0x00000000,0x00000000,
              0x00000000,0x00000000,0x00000000,0x00000000});

uint256 GEN({0x00000007,0x00000000,0x00000000,0x00000000,
              0x00000000,0x00000000,0x00000000,0x00000000});

uint256 R2({0xf3f29c6d,0xc999e990,0x87925c23,0x2b6cedcb,
            0x7254398f,0x05d31496,0x9f59ff11,0x0748d9d9});

uint32 P0INV = 4294967295;


uint32 S = 32;

// 2^32th root of unity
uint256 ROOT({0x439f0d2b,0x3829971f,0x8c2280b9,0xb6368350,
              0x22c813b4,0xd09b6819,0xdfe81f20,0x16a2a19e});

uint256 create(uint32 v) {
  return uint256({v,0,0,0,0,0,0,0});
}

void add_digit(uint32 *res, uint32 index, uint32 num) {
  while(true) {
    uint32 old = res[index];
    res[index] += num;
    if(res[index] < old) {
      num = 1;
      index++;
    } else break;
  }
}

void sub_digit(uint32 *res, uint32 index, uint32 num) {
  while(true) {
    uint32 old = res[index];
    res[index] -= num;
    if(res[index] > old) {
      num = 1;
      index++;
    } else break;
  }
}

bool gte(uint256 a, uint256 b) {
  for(int i = 7; i >= 0; i--){
    if(a.val[i] > b.val[i])
      return true;
    if(a.val[i] < b.val[i])
      return false;
  }
  return true;
}

uint256 add(uint256 a, uint256 b) {
  for(int i = 0; i < 8; i++)
    add_digit(a.val, i, b.val[i]);
  return a;
}

uint256 sub(uint256 a, uint256 b) {
  for(int i = 0; i < 8; i++)
    sub_digit(a.val, i, b.val[i]);
  return a;
}

uint256 mul_reduce(uint256 a, uint256 b) {
  uint32 res[16] = {0};
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) {
      uint64 total = (uint64)a.val[i] * (uint64)b.val[j];
      uint32 lo = total & 0xffffffff;
      uint32 hi = total >> 32;
      add_digit(res, i + j, lo);
      add_digit(res, i + j + 1, hi);
    }
  }
  for (int i = 0; i < 8; i++)
  {
    uint64 u = ((uint64)P0INV * (uint64)res[i]) & 0xffffffff;
    for(int j = 0; j < 8; j++) {
      uint64 total = u * (uint64)P.val[j];
      uint32 lo = total & 0xffffffff;
      uint32 hi = total >> 32;
      add_digit(res, i + j, lo);
      add_digit(res, i + j + 1, hi);
    }
  }
  uint256 result;
  for(int i = 0; i < 8; i++) result.val[i] = res[i+8];
  if(gte(result, P))
    result = sub(result, P);
  return result;
}

uint256 mulmod(uint256 a, uint256 b) {
  return mul_reduce(mul_reduce(mul_reduce(a, R2), mul_reduce(b, R2)), create(1));
}

uint256 negmod(uint256 a) {
  return sub(P, a);
}

uint256 submod(uint256 a, uint256 b) {
  uint256 res = sub(a, b);
  if(!gte(a, b)) res = add(res, P);
  return res;
}

uint256 addmod(uint256 a, uint256 b) {
  return submod(a, negmod(b));
}

uint256 powmod(uint256 b, uint64 p) {
  if (p == 0)
    return create(1);
  uint256 t = powmod(b, p / 2);
  t = mulmod(t, t);
  if (p % 2 == 0)
    return t;
  else
    return mulmod(b, t);
}

#endif
