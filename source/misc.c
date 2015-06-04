#include "misc.h"
#include "dsMisc.h"
#include "text.h"

unsigned long randomSeed;

int min(int x, int y) {
	return (x > y ? y : x);
}

int max(int x, int y) {
	return (x < y ? y : x);
}

int abs(int x)
{
	if (x > 0)
	{
		return x;
	}
	return -x;
}

int unitVal(int x)
{
	if (x > 0)
	{
		return 1;
	}
	if (x < 0)
	{
		return -1;
	}
	return 0;
	
}

int limitRange(int x, int lower, int upper) {
	return (min(max(x, (lower)), upper));
}


void frameTick()
{
	dice(0,10); //new random numbers every frame
	statusMessageTimeTick();
	dsFrameTick();
}

void seedRandom()
{
	randomSeed = 289301;
}

unsigned long dice(unsigned long min, unsigned long max) 
{ 
	if (randomSeed == 0)
	{
		seedRandom();
	}
	randomSeed *= 69069; 
	int result = min + ((max - min + 1) * (randomSeed >> 16)) /
				(0x1 << (4 * sizeof(unsigned long)));
	return result;
}

void shellSortReverse(void * objects[], int numbers[], int array_size)
{

  int i, j, increment, temp;
  void * objTemp;

  increment = 3;
  while (increment > 0)
  {
    for (i=0; i < array_size; i++)
    {
      j = i;
      temp = numbers[i];
	  objTemp = objects[i];
      while ((j >= increment) && (numbers[j-increment] < temp))
      {
        numbers[j] = numbers[j - increment];
		objects[j] = objects[j - increment];
        j = j - increment;
      }
      numbers[j] = temp;
	  objects[j] = objTemp;
    }
    if (increment>>1 != 0)
      increment = increment>>1;
    else if (increment == 1)
      increment = 0;
    else
      increment = 1;
  }
}

void shellSort(void * objects[], int numbers[], int array_size)
{

  int i, j, increment, temp;
  void * objTemp;

  increment = 3;
  while (increment > 0)
  {
    for (i=0; i < array_size; i++)
    {
      j = i;
      temp = numbers[i];
	  objTemp = objects[i];
      while ((j >= increment) && (numbers[j-increment] > temp))
      {
        numbers[j] = numbers[j - increment];
		objects[j] = objects[j - increment];
        j = j - increment;
      }
      numbers[j] = temp;
	  objects[j] = objTemp;
    }
    if (increment>>1 != 0)
      increment = increment>>1;
    else if (increment == 1)
      increment = 0;
    else
      increment = 1;
  }
}

SignedPair signedPair(s16 x, s16 y)
{
	SignedPair pair = 0;
	s32 tmp = x;
	if (tmp < 0)
	{
		tmp &= 0xffff;	
	}
	u32 xtemp = (u32)tmp;
	xtemp <<= 16;

	pair = xtemp;

	tmp = y;
	if (tmp < 0)
	{
		tmp &= 0xffff;
	}
	pair |= tmp;
	return pair;
}

s16 signedPairX(SignedPair pair)
{
	s16 x = 0;
	pair >>= 16;
	x |= pair;
	return x;
}

s16 signedPairY(SignedPair pair)
{
	if (pair < 0)
	{
		pair |= 0xffff0000;
	}
	else
	{
		pair &= 0x0000ffff;	
	}

	return pair;
}

bool objectIsOnScreen(int width, int height, int screenX, int screenY)
{
	if (screenX > SCREEN_WIDTH) return false;
	if (screenY > SCREEN_HEIGHT) return false;
	if (screenX < -width) return false;
	if (screenY < -height) return false;
	return true;
}

/**
 * a simple integer division routine
 * doesn't do a remainder or anything
 */
int divSmall(int value, int divBy)
{
	int answer = 0;
	while (value > divBy)
	{
		answer++;
		value -= divBy;
	}
	return answer + 1;
}

int miscContinueMainLoop;

int shouldContinueMainLoop()
{
	return miscContinueMainLoop;
	
}

void exitMainLoop()
{
	miscContinueMainLoop = false;
}

void startMainLoop()
{
	miscContinueMainLoop = true;
}



