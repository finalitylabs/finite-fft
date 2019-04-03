#!/usr/bin/python3

def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

def pvar(name,v):
    v = hex(v)[2:]
    v = '0'*(64-len(v)) + v
    v = ','.join(reversed(['0x'+str(v[i*8:i*8+8]) for i in range(8)]))
    v = 'uint256 ' + name + '({' + v + '});'
    print(v)


p = 0x73eda753299d7d483339d80809a1d80553bda402fffe5bfeffffffff00000001
root_of_unity = 0x16a2a19edfe81f20d09b681922c813b4b63683508c2280b93829971f439f0d2b
b = 2 ** 32
r = b ** 8
r2 = r * r % p
rinv = modinv(0x00000001, p)
pvar('P', p)
pvar('R2', r2)
pvar('RINV', rinv)
pvar('ROOT', root_of_unity)
rinvword = (-rinv % b)
print(rinvword)
