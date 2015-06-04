#include <nds.h>

#include "copyset.h"

void copyMem16(u16* dest, u16* src, int length) {
	//dmaCopyHalfWords(3, src, dest, length * 2);
	ndscpy(src, dest, length * 2);
}


/*
typedef struct BLOCK { u32 data[8]; } BLOCK;

void memcpy32(void *dst, const void *src, int wdcount) 
{
    u32 blkN= wdcount/8, wdN= wdcount&7;
    u32 *dstw= (u32*)dst, *srcw= (u32*)src;
    if(blkN)
    {
        // 8-word copies
        BLOCK *dst2= (BLOCK*)dst, *src2= (BLOCK*)src;
        while(blkN--)
            *dst2++ = *src2++;
        dstw= (u32*)dst2;  srcw= (u32*)src2;
    }
    // Residual words
    while(wdN--)
        *dstw++ = *srcw++;
}
*/

void copyMem32(u32* dest, u32* src, int length) {
	//memcpy32((void *)dest, (const void *) src, length);
	ndscpy(src, dest, length * 4);
}



void fillMem16(u16 value, u16* dest, int length)
{
	//const u16 * valuePtr = &value;
	//dmaFillHalfWords(valuePtr, dest, length);
	ndsset16(value, dest, length);
}

void fillMem32(u32 value, u32* dest, int length)
{
	//const u32 * valuePtr = &value;
	//dmaFillWords(valuePtr, dest, length);
	ndsset32(value, dest, length);
}

void zeroMem(u32* dest, int length)
{
	fillMem32(0, dest, length);
}
