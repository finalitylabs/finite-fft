// FinalityLabs - 2019
// 256-bit prime-field arithmetic library (addmod, submod, mulmod, powmod)
// P = 0x73eda753299d7d483339d80809a1d80553bda402fffe5bfeffffffff00000001
// Montgomery reduction parameters:
// B = 2^32 (Because our digits are uint32)
// R = B^8 = 2^256 (R and P are coprime as P is an odd prime)

typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef struct { uint32 val[8]; } uint256;

// Field Modulus
#define P ((uint256){{0x00000001,0xffffffff,0xfffe5bfe,0x53bda402,0x09a1d805,0x3339d808,0x299d7d48,0x73eda753}})

// Montgomery form of 1 = (1 * R mod P)
#define ONE ((uint256){{0xfffffffe,0x00000001,0x00034802,0x5884b7fa,0xecbc4ff5,0x998c4fef,0xacc5056f,0x1824b159}})

// -(1/P.val[0]) mod B
#define INV ((uint32)4294967295)

// Adds `num` to `i`th digit of `res` and propagates carry in case of overflow
void add_digit(uint32 *res, uint32 num) {
  uint32 old = *res;
  *res += num;
  if(*res < old) {
    res++;
    while(++(*(res++)) == 0);
  }
}

// Greater than or equal
bool gte(uint256 a, uint256 b) {
  for(int i = 7; i >= 0; i--){
    if(a.val[i] > b.val[i])
      return true;
    if(a.val[i] < b.val[i])
      return false;
  }
  return true;
}

// Normal addition
uint256 add(uint256 a, uint256 b) {
  uint32 carry = 0;
  for(int i = 0; i < 8; i++) {
    uint32 old = a.val[i];
    a.val[i] += b.val[i] + carry;
    carry = carry ? old >= a.val[i] : old > a.val[i];
  }
  return a;
}

// Normal subtraction
uint256 sub(uint256 a, uint256 b) {
  uint32 borrow = 0;
  for(int i = 0; i < 8; i++) {
    uint32 old = a.val[i];
    a.val[i] -= b.val[i] + borrow;
    borrow = borrow ? old <= a.val[i] : old < a.val[i];
  }
  return a;
}

// Modular multiplication
uint256 mulmod(uint256 a, uint256 b) {
  uint256 p = P; // TODO: Find a solution for this

  // Long multiplication
  uint32 res[16] = {0};
  for(uint32 i = 0; i < 8; i++) {
    uint32 carry = 0;
    for(uint32 j = 0; j < 8; j++) {
      uint64 product = (uint64)a.val[i] * b.val[j] + res[i + j] + carry;
      res[i + j] = product & 0xffffffff;
      carry = product >> 32;
    }
    res[i + 8] = carry;
  }

  // Montgomery reduction
  for(uint32 i = 0; i < 8; i++) {
    uint64 u = ((uint64)INV * (uint64)res[i]) & 0xffffffff;
    uint32 carry = 0;
    for(uint32 j = 0; j < 8; j++) {
      uint64 product = u * p.val[j] + res[i + j] + carry;
      res[i + j] = product & 0xffffffff;
      carry = product >> 32;
    }
    add_digit(res + i + 8, carry);
  }

  // Divide by R
  uint256 result;
  for(int i = 0; i < 8; i++) result.val[i] = res[i+8];

  if(gte(result, p))
    result = sub(result, p);

  return result;
}

// Modular negation
uint256 negmod(uint256 a) {
  uint256 p = P; // TODO: Find a solution for this
  return sub(p, a);
}

// Modular subtraction
uint256 submod(uint256 a, uint256 b) {
  uint256 p = P; // TODO: Find a solution for this
  uint256 res = sub(a, b);
  if(!gte(a, b)) res = add(res, p);
  return res;
}

// Modular addition
uint256 addmod(uint256 a, uint256 b) {
  return submod(a, negmod(b));
}

// Modular exponentiation
uint256 powmod(uint256 base, uint32 exponent) {
  uint256 res = ONE;
  while(exponent > 0) {
    if (exponent & 1)
      res = mulmod(res, base);
    exponent = exponent >> 1;
    base = mulmod(base, base);
  }
  return res;
}

uint256 powmodcached(uint256 *bases, uint32 exponent) {
  uint256 res = ONE;
  uint32 i = 0;
  while(exponent > 0) {
    if (exponent & 1)
      res = mulmod(res, bases[i]);
    exponent = exponent >> 1;
    i++;
  }
  return res;
}

uint32 bitreverse(uint32 n, uint32 bits) {
  uint32 r = 0;
  for(int i = 0; i < bits; i++) {
    r = (r << 1) | (n & 1);
    n >>= 1;
  }
  return r;
}
