#ifndef _text_h_
#define _text_h_

#include "bg.h"
#include "bgDefines.h"
#include "config.h"

#define TEXT_LINES(x) ((char **)(x))

extern Bg * textLayer;
extern Bg * subTextLayer;

void initText();

struct TextLineStruct;
typedef struct TextLineStruct TextLine;

struct TextLineStruct
{
	char ** text;
	int x;
	int y;
	TextLine * next;
};

///////////////////Necessary Includes//////////////////
#include <string.h>		//String functions


///////////////////Function Prototypes/////////
void writeLines(char ** lines);
void writeLinesPos(int x, int y, char ** lines);
void writeLinesSub(char ** lines);
void writeLinesSubPos(int x, int y, char ** lines);
void writeText(int textXLeft, int textYTop, const char *message);
void writeSubText(int textXLeft, int textYTop, const char *message);
void writeNum(int textXLeft,  int textYTop, int number);
void writeSubNum(int textXLeft,  int textYTop, int number);
void writeTile(int x, int y, int tile);
void writeTileWithPal(int x, int y, int tile, int pal);
void writeSubTile(int x, int y, int tile);
void writeSubTileWithPal(int x, int y, int tile, int pal);
void clearText();
void clearSubText();
void clearSubTextBottomHalf();
void setStatusMessageTimer(int timer);
void scrollTextBg(int x, int y);
void debugNum(int num);

#ifndef RELEASE
#define debugText(z) writeText(3,3,"ERROR"); writeText((4),(4),(z))
#else
#define debugText(z)
#endif

void statusMessageTimeTick();
void setStatusMessage(char * message);
void clearStatusMessageTimer();
void clearStatusMessage();



#endif
