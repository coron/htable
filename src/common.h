#ifndef __common_h__
#define __common_h__

typedef unsigned char byte;

void report_time(double dt,int nt,double base);
void check_ciphertext(byte *out,byte *outex,int nbyte);
double runalgo(void (*algo)(byte *,byte *,byte *),byte *in,byte *out,byte *key,byte *outex,int nbyte,int nt,double base);

#endif
