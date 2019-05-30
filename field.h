// FinalityLabs - 2019
// Arbitrary size prime-field arithmetic library (addmod, submod, mulmod, powmod)
// Montgomery reduction parameters:
// B = 2^32 (Because our digits are uint32)

typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef struct { uint32 val[LIMBS]; } field;

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
bool gte(field a, field b) {
  for(int i = LIMBS - 1; i >= 0; i--){
    if(a.val[i] > b.val[i])
      return true;
    if(a.val[i] < b.val[i])
      return false;
  }
  return true;
}

// Equals
bool eq(field a, field b) {
  for(int i = 0; i < LIMBS; i++)
    if(a.val[i] != b.val[i])
      return false;
  return true;
}

// Normal addition
field add(field a, field b) {
  uint32 carry = 0;
  for(int i = 0; i < LIMBS; i++) {
    uint32 old = a.val[i];
    a.val[i] += b.val[i] + carry;
    carry = carry ? old >= a.val[i] : old > a.val[i];
  }
  return a;
}

// Normal subtraction
field sub(field a, field b) {
  uint32 borrow = 0;
  for(int i = 0; i < LIMBS; i++) {
    uint32 old = a.val[i];
    a.val[i] -= b.val[i] + borrow;
    borrow = borrow ? old <= a.val[i] : old < a.val[i];
  }
  return a;
}

// Modular multiplication
field mulmod(field a, field b) {
  field p = P; // TODO: Find a solution for this

  // Long multiplication
  uint32 res[LIMBS * 2] = {0};
  for(uint32 i = 0; i < LIMBS; i++) {
    uint32 carry = 0;
    for(uint32 j = 0; j < LIMBS; j++) {
      uint64 product = (uint64)a.val[i] * b.val[j] + res[i + j] + carry;
      res[i + j] = product & 0xffffffff;
      carry = product >> 32;
    }
    res[i + LIMBS] = carry;
  }

  // Montgomery reduction
  for(uint32 i = 0; i < LIMBS; i++) {
    uint64 u = ((uint64)INV * (uint64)res[i]) & 0xffffffff;
    uint32 carry = 0;
    for(uint32 j = 0; j < LIMBS; j++) {
      uint64 product = u * p.val[j] + res[i + j] + carry;
      res[i + j] = product & 0xffffffff;
      carry = product >> 32;
    }
    add_digit(res + i + LIMBS, carry);
  }

  // Divide by R
  field result;
  for(int i = 0; i < LIMBS; i++) result.val[i] = res[i+LIMBS];

  if(gte(result, p))
    result = sub(result, p);

  return result;
}

// Modular negation
field negmod(field a) {
  field p = P; // TODO: Find a solution for this
  return sub(p, a);
}

// Modular subtraction
field submod(field a, field b) {
  field p = P; // TODO: Find a solution for this
  field res = sub(a, b);
  if(!gte(a, b)) res = add(res, p);
  return res;
}

// Modular addition
field addmod(field a, field b) {
  return submod(a, negmod(b));
}

// Modular exponentiation
field powmod(field base, uint32 exponent) {
  field res = ONE;
  while(exponent > 0) {
    if (exponent & 1)
      res = mulmod(res, base);
    exponent = exponent >> 1;
    base = mulmod(base, base);
  }
  return res;
}

field powmodcached(field *bases, uint32 exponent) {
  field res = ONE;
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
