// FinalityLabs - 2019
// 384-bit prime-field arithmetic library (addmod, submod, mulmod, powmod)
// P = 0x1a0111ea397fe69a4b1ba7b6434bacd764774b84f38512bf6730d2a0f6b0f6241eabfffeb153ffffb9feffffffffaaab
// Montgomery reduction parameters:
// B = 2^32 (Because our digits are uint32)
// R = B^12 = 2^384 (R and P are coprime as P is an odd prime)

typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef struct { uint32 val[12]; } uint384;

// Field Modulus
#define P ((uint384){{0xffffaaab,0xb9feffff,0xb153ffff,0x1eabfffe,0xf6b0f624,0x6730d2a0,0xf38512bf,0x64774b84,0x434bacd7,0x4b1ba7b6,0x397fe69a,0x1a0111ea}})

// Montgomery form of 1 = (1 * R mod P)
#define ONE ((uint384){{0x0002fffd,0x76090000,0xc40c0002,0xebf4000b,0x53c758ba,0x5f489857,0x70525745,0x77ce5853,0xa256ec6d,0x5c071a97,0xfa80e493,0x15f65ec3}})

#define ZERO ((uint384){{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}})

// -(1/P.val[0]) mod B
#define INV ((uint32)4294770685)

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
bool eq(uint384 a, uint384 b) {
  for(int i = 11; i >= 0; i--)
    if(a.val[i] != b.val[i])
      return false;
  return true;
}

// Greater than or equal
bool gte(uint384 a, uint384 b) {
  for(int i = 11; i >= 0; i--){
    if(a.val[i] > b.val[i])
      return true;
    if(a.val[i] < b.val[i])
      return false;
  }
  return true;
}

// Normal addition
uint384 add(uint384 a, uint384 b) {
  uint32 carry = 0;
  for(int i = 0; i < 12; i++) {
    uint32 old = a.val[i];
    a.val[i] += b.val[i] + carry;
    carry = carry ? old >= a.val[i] : old > a.val[i];
  }
  return a;
}

// Normal subtraction
uint384 sub(uint384 a, uint384 b) {
  uint32 borrow = 0;
  for(int i = 0; i < 12; i++) {
    uint32 old = a.val[i];
    a.val[i] -= b.val[i] + borrow;
    borrow = borrow ? old <= a.val[i] : old < a.val[i];
  }
  return a;
}

// Modular multiplication
uint384 mulmod(uint384 a, uint384 b) {
  uint384 p = P; // TODO: Find a solution for this

  // Long multiplication
  uint32 res[24] = {0};
  for(uint32 i = 0; i < 12; i++) {
    uint32 carry = 0;
    for(uint32 j = 0; j < 12; j++) {
      uint64 product = (uint64)a.val[i] * b.val[j] + res[i + j] + carry;
      res[i + j] = product & 0xffffffff;
      carry = product >> 32;
    }
    res[i + 12] = carry;
  }

  // Montgomery reduction
  for(uint32 i = 0; i < 12; i++) {
    uint64 u = ((uint64)INV * (uint64)res[i]) & 0xffffffff;
    uint32 carry = 0;
    for(uint32 j = 0; j < 12; j++) {
      uint64 product = u * p.val[j] + res[i + j] + carry;
      res[i + j] = product & 0xffffffff;
      carry = product >> 32;
    }
    add_digit(res + i + 12, carry);
  }

  // Divide by R
  uint384 result;
  for(int i = 0; i < 12; i++) result.val[i] = res[i+12];

  if(gte(result, p))
    result = sub(result, p);

  return result;
}

// Modular negation
uint384 negmod(uint384 a) {
  uint384 p = P; // TODO: Find a solution for this
  return sub(p, a);
}

// Modular subtraction
uint384 submod(uint384 a, uint384 b) {
  uint384 p = P; // TODO: Find a solution for this
  uint384 res = sub(a, b);
  if(!gte(a, b)) res = add(res, p);
  return res;
}

// Modular addition
uint384 addmod(uint384 a, uint384 b) {
  return submod(a, negmod(b));
}

// Modular exponentiation
uint384 powmod(uint384 base, uint32 exponent) {
  uint384 res = ONE;
  while(exponent > 0) {
    if (exponent & 1)
      res = mulmod(res, base);
    exponent = exponent >> 1;
    base = mulmod(base, base);
  }
  return res;
}

uint384 powmodcached(uint384 *bases, uint32 exponent) {
  uint384 res = ONE;
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
