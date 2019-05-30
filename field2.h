#define ZERO2 ((field2){ZERO, ZERO})
#define ONE2 ((field2){ONE, ZERO})

typedef struct {
  field c0;
  field c1;
} field2;

bool eq2(field2 a, field2 b) {
  return eq(a.c0, b.c0) && eq(a.c1, b.c1);
}
field2 negmod2(field2 a) {
  a.c0 = negmod(a.c0);
  a.c1 = negmod(a.c1);
  return a;
}
field2 submod2(field2 a, field2 b) {
  a.c0 = submod(a.c0, b.c0);
  a.c1 = submod(a.c1, b.c1);
  return a;
}
field2 addmod2(field2 a, field2 b) {
  a.c0 = addmod(a.c0, b.c0);
  a.c1 = addmod(a.c1, b.c1);
  return a;
}
field2 mulmod2(field2 a, field2 b) {
  field aa = mulmod(a.c0, b.c0);
  field bb = mulmod(a.c1, b.c1);
  field o = addmod(b.c0, b.c1);
  a.c1 = addmod(a.c1, a.c0);
  a.c1 = mulmod(a.c1, o);
  a.c1 = submod(a.c1, aa);
  a.c1 = submod(a.c1, bb);
  a.c0 = submod(aa, bb);
  return a;
}
