#include "text.h"
#include "misc.h"
#include "gfx/hud.h"
#include "bg.h"
#include "enemies/itemGfx.h"
#include "hud.h"
#include "subscreen.h"
#include "memcpy.h"

extern const unsigned short alphabet_graphics;

Bg * textLayer;
Bg * subTextLayer;

Bg textLayerBg;
Bg subTextLayerBg;

void initText()
{
	textLayer = &textLayerBg;
	initBackground(textLayer, 0);
	textLayer->tileBlock = 2;
	textLayer->mapBlock = 24;
	textLayer->size = BgSize_T_256x256;
	textLayer->colorMode = BG_COLOR_16;
	textLayer->isSub = false;

	subTextLayer = &subTextLayerBg;
	initBackground(subTextLayer, 0);
	subTextLayer->tileBlock = 2;
	subTextLayer->mapBlock = 24;
	subTextLayer->size = BgSize_T_256x256;
	subTextLayer->colorMode = BG_COLOR_16;
	subTextLayer->isSub = true;


	frameTick();
	enableBackground(textLayer);
	enableBackground(subTextLayer);
	zeroBGMap(textLayer, 32*2);
	zeroBGMap(subTextLayer, 32*2);
	loadBGTiles(textLayer, (u32*)&hud_Tiles, 400);
	loadBGTiles(subTextLayer, (u32*)&hud_Tiles, 400);
	reloadHudPalette();

}

void scrollTextBg(int x, int y)
{
	textLayerBg.x_scroll = x;
	textLayerBg.y_scroll = y;
	updateBackground(&textLayerBg);
}

void writeLines(char ** lines)
{
	writeLinesPos(1,1,lines);
}

void writeLinesPos(int x, int y, char ** lines)
{
	reloadHudPalette();
	while ((*lines != null) && (y < 32))
	{
		writeText(x, y, *lines);
		y++;
		lines++;
	}
	
}

void writeLinesSub(char ** lines)
{
	writeLinesSubPos(1,1,lines);
}

void writeLinesSubPos(int x, int y, char ** lines)
{
	reloadHudPalette();
	while ((*lines != null) && (y < 32))
	{
		writeSubText(x, y, *lines);
		y++;
		lines++;
	}
	
}


//////////////////////Globals////////////////////////
#define SPACE 0x00

#define STATUS_MSG_X 2
#define STATUS_MSG_Y 4
#define STATUS_MSG_DISPLAY_TIME 120

int text_status_msg_timer;


void clearStatusMessage()
{
	writeSubText(STATUS_MSG_X, STATUS_MSG_Y,"                          ");
	clearEnemyPortrait();
	writeMidScreenTile(STATUS_MSG_X - 1, STATUS_MSG_Y - 1, TILE_BLACK);
	writeMidScreenTile(STATUS_MSG_X - 1, STATUS_MSG_Y + 1, TILE_BLACK);
	writeMidScreenTile(STATUS_MSG_X - 1, STATUS_MSG_Y , TILE_BLACK);
	int i;
	for (i = 0; i < 32; i++)
	{
		writeMidScreenTile(i, STATUS_MSG_Y - 1, TILE_BLACK);
		writeMidScreenTile(i, STATUS_MSG_Y + 1, TILE_BLACK);
		writeMidScreenTile(i, STATUS_MSG_Y, TILE_BLACK);
	}
	writeMidScreenTile(i, STATUS_MSG_Y - 1, TILE_BLACK);
	writeMidScreenTile(i, STATUS_MSG_Y + 1, TILE_BLACK);
	writeMidScreenTile(i, STATUS_MSG_Y , TILE_BLACK);

}

void setStatusMessage(char * message)
{
	clearStatusMessage();
	clearEnemyPortrait();
	writeSubText(STATUS_MSG_X, STATUS_MSG_Y,"                      ");
	writeSubText(STATUS_MSG_X, STATUS_MSG_Y, message);

	//write left edge of box...
	writeMidScreenTile(STATUS_MSG_X - 1, STATUS_MSG_Y - 1, TILE_TL_CORNER);
	writeMidScreenTile(STATUS_MSG_X - 1, STATUS_MSG_Y + 1, TILE_BL_CORNER);
	writeMidScreenTile(STATUS_MSG_X - 1, STATUS_MSG_Y , TILE_L_EDGE);

	//for each characters, write a column
	char * chr = message;
	int i = STATUS_MSG_X;
	while (*chr++ != '\0')
	{
		writeMidScreenTile(i, STATUS_MSG_Y - 1, TILE_T_EDGE);
		writeMidScreenTile(i, STATUS_MSG_Y, TILE_BLACK);
		writeMidScreenTile(i, STATUS_MSG_Y + 1, TILE_B_EDGE);
		i++;
	}

	//write left edge of box...
	writeMidScreenTile(i, STATUS_MSG_Y - 1, TILE_TR_CORNER);
	writeMidScreenTile(i, STATUS_MSG_Y + 1, TILE_BR_CORNER);
	writeMidScreenTile(i, STATUS_MSG_Y , TILE_R_EDGE);

	showMidScreen();
	text_status_msg_timer = 120;
}

void clearStatusMessageTimer()
{
	text_status_msg_timer = 0;
}

void statusMessageTimeTick()
{
	if (text_status_msg_timer > 0)
	{
		text_status_msg_timer--;
		if (text_status_msg_timer == 0)
		{
			//TODO: check for infinite looping on setStatusMessage
			//setStatusMessage("                                 ");
			clearStatusMessage();
		}
	}
}

void setStatusMessageTimer(int timer)
{
	text_status_msg_timer = timer;
}



///////////////////writeText/////////////////
void writeTextWithDest(int textXLeft, int textYTop,  const char *message, u16* destination);

void writeText(int textXLeft, int textYTop,  const char *message)
{
	u16* BGPaletteMem = getBgPaletteMem();
	BGPaletteMem[254] = 0;
	BGPaletteMem[255] = 0xFFFF;
	writeTextWithDest(textXLeft, textYTop, message, textLayer->mapData);
}

void writeSubText(int textXLeft, int textYTop,  const char *message)
{
	u16* BGPaletteMem = getSubBgPaletteMem();
	BGPaletteMem[254] = 0;
	BGPaletteMem[255] = 0xFFFF;
	writeTextWithDest(textXLeft, textYTop, message, subTextLayer->mapData);
}

void writeTextWithDest(int textXLeft, int textYTop,  const char *message, u16* destination)
{
	int currentLetter;				//Current letter
	int j = 0;					//Looping variables
	int xOff = 0;					//X Column
	int length = 0;					//Array length
	int newString[600];				//Hex version of the string


	while(message[j] != '\0')
	{
		switch(message[j])
		{
			case 'A': newString[j] = 0x01; break;
			case 'B': newString[j] = 0x02; break;
			case 'C': newString[j] = 0x03; break;
			case 'D': newString[j] = 0x04; break;
			case 'E': newString[j] = 0x05; break;
			case 'F': newString[j] = 0x06; break;
			case 'G': newString[j] = 0x07; break;
			case 'H': newString[j] = 0x08; break;
			case 'I': newString[j] = 0x09; break;
			case 'J': newString[j] = 0x0A; break;
			case 'K': newString[j] = 0x0B; break;
			case 'L': newString[j] = 0x0C; break;
			case 'M': newString[j] = 0x0D; break;
			case 'N': newString[j] = 0x0E; break;
			case 'O': newString[j] = 0x0F; break;
			case 'P': newString[j] = 0x10; break;
			case 'Q': newString[j] = 0x11; break;
			case 'R': newString[j] = 0x12; break;
			case 'S': newString[j] = 0x13; break;
			case 'T': newString[j] = 0x14; break;
			case 'U': newString[j] = 0x15; break;
			case 'V': newString[j] = 0x16; break;
			case 'W': newString[j] = 0x17; break;
			case 'X': newString[j] = 0x18; break;
			case 'Y': newString[j] = 0x19; break;
			case 'Z': newString[j] = 0x1A; break;
			case 'a': newString[j] = 0x1B; break;
			case 'b': newString[j] = 0x1C; break;
			case 'c': newString[j] = 0x1D; break;
			case 'd': newString[j] = 0x1E; break;
			case 'e': newString[j] = 0x1F; break;
			case 'f': newString[j] = 0x20; break;
			case 'g': newString[j] = 0x21; break;
			case 'h': newString[j] = 0x22; break;
			case 'i': newString[j] = 0x23; break;
			case 'j': newString[j] = 0x24; break;
			case 'k': newString[j] = 0x25; break;
			case 'l': newString[j] = 0x26; break;
			case 'm': newString[j] = 0x27; break;
			case 'n': newString[j] = 0x28; break;
			case 'o': newString[j] = 0x29; break;
			case 'p': newString[j] = 0x2A; break;
			case 'q': newString[j] = 0x2B; break;
			case 'r': newString[j] = 0x2C; break;
			case 's': newString[j] = 0x2D; break;
			case 't': newString[j] = 0x2E; break;
			case 'u': newString[j] = 0x2F; break;
			case 'v': newString[j] = 0x30; break;
			case 'w': newString[j] = 0x31; break;
			case 'x': newString[j] = 0x32; break;
			case 'y': newString[j] = 0x33; break;
			case 'z': newString[j] = 0x34; break;
			case '0': newString[j] = 0x35; break;
			case '1': newString[j] = 0x36; break;
			case '2': newString[j] = 0x37; break;
			case '3': newString[j] = 0x38; break;
			case '4': newString[j] = 0x39; break;
			case '5': newString[j] = 0x3A; break;
			case '6': newString[j] = 0x3B; break;
			case '7': newString[j] = 0x3C; break;
			case '8': newString[j] = 0x3D; break;
			case '9': newString[j] = 0x3E; break;
			case '!': newString[j] = 0x3F; break;
			//case '"': newString[j] = 0x40; break;
			case '#': newString[j] = 0x41; break;
			case '$': newString[j] = 0x42; break;
			case '%': newString[j] = 0x43; break;
			case '&': newString[j] = 0x44; break;
			//case ''': newString[j] = 0x45; break;
			case '(': newString[j] = 0x46; break;
			case ')': newString[j] = 0x47; break;
			case '*': newString[j] = 0x48; break;
			case '+': newString[j] = 0x49; break;
			case ',': newString[j] = 0x4A; break;
			case '-': newString[j] = 0x4B; break;
			case '.': newString[j] = 0x4C; break;
			case '/': newString[j] = 0x4D; break;
			case ':': newString[j] = 0x4E; break;
			case ';': newString[j] = 0x4F; break;
			case '<': newString[j] = 0x50; break;
			case '=': newString[j] = 0x51; break;
			case '>': newString[j] = 0x52; break;
			case '?': newString[j] = 0x53; break;


			default:  newString[j] = SPACE; break;
		}

		j++;
	}

	length = j;

	newString[j] = 0x00;
	newString[j+1] = 0x00;

	int pal = HUD_PAL;

	for(currentLetter = 0; currentLetter <= length; currentLetter++)
	{

		destination[(textYTop)*32 + textXLeft + xOff] = 
				 (newString[currentLetter]) | pal;
		xOff++;
	}
}

void writeTile(int x, int y, int tile)
{
	textLayer->mapData[y*32 + x] = tile | HUD_PAL;
}

void writeSubTile(int x, int y, int tile)
{
	subTextLayer->mapData[y*32 + x] = tile | HUD_PAL;
}

void writeTileWithPal(int x, int y, int tile, int pal)
{
	textLayer->mapData[y*32 + x] = tile | pal;
}

void writeSubTileWithPal(int x, int y, int tile, int pal)
{
	subTextLayer->mapData[y*32 + x] = tile | pal;
}

///helper for writeNum
char u32toaChar[12];
void u32toa(u32 value, char* output) {

   char u32toaChar[12];   //enough for max numChars required
   u32 v = value;
   u32 oldv = 0;
   s32 chIdx = 0;         
   do{      
      oldv = v;
      v /= 10;    //compiler should optimise divide by constant - best to check asm output
      u32toaChar[chIdx] = (oldv - (v*10) + 48);   //48 is start of numbers in ASCII
      ++chIdx;
   }while(v);
   //we have our decimal digits but in reverse order... so re-order them
   s32 outIdx = 0;
   do{
      --chIdx;
      output[outIdx] = u32toaChar[chIdx];
      ++outIdx;
   }while(chIdx > 0);
   output[outIdx] = 0;   //terminating zero
   return;
} 

char numString[17];

//////////////////writeNum//////////////////
void writeNum(int textXLeft, int textYTop, int number)
{

	//sprintf(numString, "%d", number);	//Convert
	if (number < 0)
	{
		s32 neg = -number;
		u32 num = neg;
		u32toa(((u32)num) , numString);
		writeText(textXLeft , textYTop, "-");
		writeText(textXLeft + 1, textYTop, numString);
	}
	else
	{
		u32toa(((u32)number) , numString);
		writeText(textXLeft, textYTop, numString);
	}
}

void clearText()
{
	zeroMem((u32*)textLayer->mapData, 2048);
	forceHudRedraw();
	clearStatusMessageTimer();
}

void writeSubNum(int textXLeft, int textYTop, int number)
{

	//sprintf(numString, "%d", number);	//Convert
	if (number < 0)
	{
		s32 neg = -number;
		u32 num = neg;
		u32toa(((u32)num) , numString);
		writeSubText(textXLeft , textYTop, "-");
		writeSubText(textXLeft + 1, textYTop, numString);
	}
	else
	{
		u32toa(((u32)number) , numString);
		writeSubText(textXLeft, textYTop, numString);
	}
}

void clearSubText()
{
	zeroMem((u32*)subTextLayer->mapData, 2048);
	forceHudRedraw();
	clearStatusMessageTimer();
}

void clearSubTextBottomHalf()
{
	zeroMem((u32*)subTextLayer->mapData + 1024, 1024);
}


int debugTxtLine = 2;
int debugTxtCol = 2;
void debugNum(int num)
{
	writeSubText(0 + debugTxtCol,debugTxtLine, " ");
	if (debugTxtLine++ > 16)
	{
		debugTxtLine = 2;
		debugTxtCol += 8;
		if (debugTxtCol > 20)
		{
			debugTxtCol = 2;
		}
	}
	writeSubText(1 + debugTxtCol,debugTxtLine, ">");
	writeSubNum(2 + debugTxtCol,debugTxtLine, num);
}



