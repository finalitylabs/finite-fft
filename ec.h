typedef struct {
  uint384 x;
  uint384 y;
  bool inf;
} affine;

typedef struct {
  uint384 x;
  uint384 y;
  uint384 z;
} projective;

projective doub(projective inp) {
  if(eq(inp.z, ZERO)) return inp;
  uint384 a = mulmod(inp.x, inp.x); // A = X1^2
  uint384 b = mulmod(inp.y, inp.y); // B = Y1^2
  uint384 c = mulmod(b, b); // C = B^2

  // D = 2*((X1+B)2-A-C)
  uint384 d = addmod(inp.x, b);
  d = mulmod(d, d); d = submod(submod(d, a), c); d = addmod(d, d);

  uint384 e = addmod(addmod(a, a), a); // E = 3*A

  uint384 f = mulmod(e, e);


  inp.z = mulmod(inp.y, inp.z); inp.z = addmod(inp.z, inp.z); // Z3 = 2*Y1*Z1
  inp.x = submod(submod(f, d), d); // X3 = F-2*D

  // Y3 = E*(D-X3)-8*C
  c = addmod(c, c); c = addmod(c, c); c = addmod(c, c);
  inp.y = submod(mulmod(submod(d, inp.x), e), c);

  return inp;
}

projective add(projective a, affine b) {
  if(b.inf) return a;

  if(eq(a.z, ZERO)) {
    a.x = b.x;
    a.y = b.y;
    a.z = ONE;
    return a;
  }

  uint384 z1z1 = mulmod(a.z, a.z);
  uint384 u2 = mulmod(b.x, z1z1);
  uint384 s2 = mulmod(mulmod(b.y, a.z), z1z1);

  if(eq(a.x, u2) && eq(b.y, s2))
    return doub(a);
  else {
    uint384 h = submod(u2, a.x); // H = U2-X1
    uint384 hh = mulmod(h, h); // HH = H^2
    uint384 i = addmod(hh, hh); i = addmod(i, i); // I = 4*HH
    uint384 j = mulmod(h, i); // J = H*I
    uint384 r = submod(s2, a.y); r = addmod(r, r); // r = 2*(S2-Y1)
    uint384 v = mulmod(a.x, i);

    projective ret;

     // X3 = r^2 - J - 2*V
    ret.x = submod(submod(mulmod(r, r), j), addmod(v, v));

     // Y3 = r*(V-X3)-2*Y1*J
    j = mulmod(a.y, j); j = addmod(j, j);
    ret.y = submod(mulmod(submod(v, ret.x), r), j);

    // Z3 = (Z1+H)^2-Z1Z1-HH
    ret.z = addmod(a.z, h); ret.z = submod(submod(mulmod(ret.z, ret.z), z1z1), hh);
    return ret;
  }
}
