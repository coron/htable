#ifndef __share_h__
#define __share_h__

#include "common.h"

uint32_t xorshf96(void);

void refresh(byte a[],int n);
void share(byte x,byte a[],int n);
byte xorop(byte a[],int n);
byte decode(byte a[],int n);

#endif
