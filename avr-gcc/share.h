#ifndef __share_h__
#define __share_h__

#include "common.h"

uint32_t xorshf96(void);

void refresh(byte a[],uint8_t n);
void share(byte x,byte a[],uint8_t n);
byte xorop(byte a[],uint8_t n);
byte decode(byte a[],uint8_t n);

#endif
