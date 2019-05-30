typedef struct {
  field2 x;
  field2 y;
  bool inf;
} affine2;

typedef struct {
  field2 x;
  field2 y;
  field2 z;
} projective2;

projective2 ec2_double(projective2 inp) {
  if(eq2(inp.z, ZERO2)) return inp;
  field2 a = mulmod2(inp.x, inp.x); // A = X1^2
  field2 b = mulmod2(inp.y, inp.y); // B = Y1^2
  field2 c = mulmod2(b, b); // C = B^2

  // D = 2*((X1+B)2-A-C)
  field2 d = addmod2(inp.x, b);
  d = mulmod2(d, d); d = submod2(submod2(d, a), c); d = addmod2(d, d);

  field2 e = addmod2(addmod2(a, a), a); // E = 3*A

  field2 f = mulmod2(e, e);

  inp.z = mulmod2(inp.y, inp.z); inp.z = addmod2(inp.z, inp.z); // Z3 = 2*Y1*Z1
  inp.x = submod2(submod2(f, d), d); // X3 = F-2*D

  // Y3 = E*(D-X3)-8*C
  c = addmod2(c, c); c = addmod2(c, c); c = addmod2(c, c);
  inp.y = submod2(mulmod2(submod2(d, inp.x), e), c);

  return inp;
}

projective2 ec2_add(projective2 a, affine2 b) {
  if(b.inf) return a;

  if(eq2(a.z, ZERO2)) {
    a.x = b.x;
    a.y = b.y;
    a.z = ONE2;
    return a;
  }

  field2 z1z1 = mulmod2(a.z, a.z);
  field2 u2 = mulmod2(b.x, z1z1);
  field2 s2 = mulmod2(mulmod2(b.y, a.z), z1z1);

  if(eq2(a.x, u2) && eq2(b.y, s2))
    return ec2_double(a);
  else {
    field2 h = submod2(u2, a.x); // H = U2-X1
    field2 hh = mulmod2(h, h); // HH = H^2
    field2 i = addmod2(hh, hh); i = addmod2(i, i); // I = 4*HH
    field2 j = mulmod2(h, i); // J = H*I
    field2 r = submod2(s2, a.y); r = addmod2(r, r); // r = 2*(S2-Y1)
    field2 v = mulmod2(a.x, i);

    projective2 ret;

     // X3 = r^2 - J - 2*V
    ret.x = submod2(submod2(mulmod2(r, r), j), addmod2(v, v));

     // Y3 = r*(V-X3)-2*Y1*J
    j = mulmod2(a.y, j); j = addmod2(j, j);
    ret.y = submod2(mulmod2(submod2(v, ret.x), r), j);

    // Z3 = (Z1+H)^2-Z1Z1-HH
    ret.z = addmod2(a.z, h); ret.z = submod2(submod2(mulmod2(ret.z, ret.z), z1z1), hh);
    return ret;
  }
}

//G2(x=Fq2(Fq(0x07545128dec4be8d440d067bf6994c23aefbd182fe472e30c1403e45e13c40df2423bf00a0356d4a1c15efec886a3d44) + Fq(0x028e3c3a3725e57ed818eda1a73b37cc7e816c64d5e669c01e53d1b01080ce2a1edc72a3fe601bb9412feab33a4b00b5) * u), y=Fq2(Fq(0x15296affe397343a68398a9d13d3a2afbd2412fcb9ceb706f1180acde6a35c7fd24f3956913b1bef0afad9e53ee0fd04) + Fq(0x1617815e4e474861d7eb038418d34e8d2e658c1266853b699ac0af2f2fb40b7634d07689875ac2a8e9ce3c7e820748dc) * u))
//0x0000000000000000000000000000000000000000000000000000000000000002
//G2(x=Fq2(Fq(0x05bbca41df309b17cbd3f3e18a01a910d8f78d577918fda64857c8dfc715267fff1d371008c4014c06e8a45a7008757f) + Fq(0x0c1f5f2e00242ace242b95d968f23ed0acd73a33af15405f036937cccf66b45d8c481d9f8e9d0e7c1ebe584e36f0b653) * u), y=Fq2(Fq(0x09e8a5d902fea52a9ff458b33b79e387f8cb850d85a30bfdf7fbb935216534edad99f1d9019612d5e577972a7ed65731) + Fq(0x14baa2bd38545bd2d88abbf3750fe9c0d930f462ed46d6c1d57188a6043ef9169b208637f18a67c21406d22922c9d13d) * u))
