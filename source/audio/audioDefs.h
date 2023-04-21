#ifndef AUDIODEFS_H_
#define AUDIODEFS_H_

#include "audio/wavs/thunk_wav.h"
#include "audio/wavs/potion_wav.h"
#include "audio/wavs/armor_wav.h"
#include "audio/wavs/bite_wav.h"
#include "audio/wavs/changeItem_wav.h"
#include "audio/wavs/door_wav.h"
#include "audio/wavs/getItemGood_wav.h"
#include "audio/wavs/pageFlip_wav.h"
#include "audio/wavs/spit_wav.h"
#include "audio/wavs/swingSword_wav.h"
#include "audio/wavs/ugh_wav.h"
#include "audio/wavs/armor_wav.h"
#include "audio/wavs/dynamite_wav.h"


// TODO asie
//#define DEFSFX(x) extern const u8 x##_wav[];
#define DEFSFX(x)

#define REFSONG(x)  x##_bin;
#define REFSONG_SIZE(x)  x##_bin_size;

#define REFSFX(x)  (void*)x##_wav;
#define REFSFX_SIZE(x)  x##_wav_size;


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
