#ifndef __aes_h__
#define __aes_h__

#include "common.h"

extern const byte sbox[256];

byte multx(byte x);
byte mult(byte x,byte y);
byte inverse(byte x);

byte bit(byte x,uint8_t i);
byte affine(byte x);

byte subbyte(byte x);
void printstate(byte state[16]);

void shiftrows(byte state[16]);
void addroundkey(byte *state,byte *w,uint8_t round);

void aes(byte in[16],byte out[16],byte key[16]);
double run_aes(byte in[16],byte out[16],byte key[16],uint8_t nt);

void testaes();

void keyexpansion(byte *key,byte *w);

#endif
