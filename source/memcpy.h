#ifndef MEMCPY_H_
#define MEMCPY_H_

void copyMem16(u16* dest, u16* src, int length);
void fillMem16(u16 value, u16* dest, int length);
void copyMem32(u32* dest, u32* src, int length);
void zeroMem(u32* dest, int length);

#endif /*MEMCPY_H_*/
