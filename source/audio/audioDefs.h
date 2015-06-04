#ifndef AUDIODEFS_H_
#define AUDIODEFS_H_

#include "wavs/thunk.h"
#include "wavs/potion.h"
#include "wavs/armor.h"
#include "wavs/bite.h"
#include "wavs/changeItem.h"
#include "wavs/door.h"
#include "wavs/getItemGood.h"
#include "wavs/pageFlip.h"
#include "wavs/spit.h"
#include "wavs/swingSword.h"
#include "wavs/ugh.h"
#include "wavs/armor.h"
#include "wavs/dynamite.h"


#define DEFSFX(x) extern const u8 x##_wav[]; 


#define REFSONG(x)  x##_bin;
#define REFSONG_SIZE(x)  x##_bin_size;

#define REFSFX(x)  (void*)x##_wav;
#define REFSFX_SIZE(x)  x##_wav_length;


//DEFSONG(dungeon);
//DEFSONG(overworld);
//DEFSONG(fred);

DEFSFX(thunk);
DEFSFX(potion);
DEFSFX(armor);
DEFSFX(bite);
DEFSFX(changeItem);
DEFSFX(door);
DEFSFX(getItemGood);
DEFSFX(pageFlip);
DEFSFX(spit);
DEFSFX(swingSword);
DEFSFX(ugh);
DEFSFX(armor);
DEFSFX(dynamite);

				   


#endif
