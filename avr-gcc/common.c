#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void report_time(double dt,int nt,double base)
{
  printf("%f %f\n",((double) dt)/CLOCKS_PER_SEC/nt*1000,dt/base);
}

void check_ciphertext(byte *out,byte *outex,int nbyte)
{
  if(memcmp(out,outex,nbyte)!=0)
  {
    fprintf(stderr,"Error: incorrect ciphertext\n");
    exit(EXIT_FAILURE);
  }
}

double runalgo(void (*algo)(byte *,byte *,byte *),byte *in,byte *out,byte *key,byte *outex,int nbyte,int nt,double base)
{
  int i;
  clock_t start,end;
  double dt;
  start=clock();
  for(i=0;i<nt;i++)
    algo(in,out,key);
  end=clock();
  dt=(double) (end-start);
  if (base==0.0f) base=dt;
  report_time(dt,nt,base);
  check_ciphertext(out,outex,nbyte);
  return dt;
}


