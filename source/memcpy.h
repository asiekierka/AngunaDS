#ifndef MEMCPY_H_
#define MEMCPY_H_

#include "copyset.h"

static inline void copyMem16(u16* dest, u16* src, int length)
{
	//dmaCopyHalfWords(3, src, dest, length * 2);
	memcpy(dest, src, length * 2);
}


static inline void copyMem32(u32* dest, u32* src, int length)
{
	memcpy(dest, src, length * 4);
}

static inline void fillMem16(u16 value, u16* dest, int length)
{
	//const u16 * valuePtr = &value;
	//dmaFillHalfWords(valuePtr, dest, length);
	ndsset16(value, dest, length);
}

static inline void fillMem32(u32 value, u32* dest, int length)
{
	//const u32 * valuePtr = &value;
	//dmaFillWords(valuePtr, dest, length);
	ndsset32(value, dest, length);
}

static inline void zeroMem(u32* dest, int length)
{
	fillMem32(0, dest, length);
}

#endif /*MEMCPY_H_*/
