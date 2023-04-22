#ifndef _COPY_SET_H_
#define _COPY_SET_H_

#include <nds.h>

//from cearn's ndsset.s

void ndsset  (u8  src, void *dstv, unsigned int size);
void ndsset16(u16 src, void *dstv, unsigned int size);
void ndsset32(u32 src, void *dstv, unsigned int size);
#endif
