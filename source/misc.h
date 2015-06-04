#ifndef MISC_H_
#define MISC_H_

#include "platform.h"

//we need a handy null
#define null 0

//and I hate errors about punning types, when I do it on purpose...
#define PUN(ptr) (ptr)(void*)

int min(int x, int y);
int max(int x, int y);
int abs(int x);
int unitVal(int x);
int limitRange(int x, int lower, int upper);

/**
 * Do some sort of debug output...
 */
void debugText(char * debugText);


/**
 * Seeds the random number generator
 */
void seedRandom();

/**
 * Returns a random "dice roll" between min and max.
 * Also advances the pseudo-random-number generator
 */
unsigned long dice(unsigned long min, unsigned long max) ;


/**
 * Sort objects based on the array of numbers
 */
void shellSort(void * objects[], int numbers[], int array_size);

/**
 * Sort objects backwards based on the array of numbers
 */
void shellSortReverse(void * objects[], int numbers[], int array_size);

/**
 * Using a u32 to return a signed pair of 16 bit numbers, 
 * so we can pass them around as a single object, return
 * from functions, etc, without having to do any fancy
 * memory allocation
 */
typedef u32 SignedPair;

SignedPair signedPair(s16 x, s16 y);
s16 signedPairX(SignedPair pair);
s16 signedPairY(SignedPair pair);

/**
 * And a Point data-type that is a first class struct.  
 * X,Y aren't limited to 16 bits here...
 */
typedef struct
{
	int x;
	int y;
} Point;

/**
 * Determines whether an object is onscreen
 */
bool objectIsOnScreen(int width, int height, int screenX, int screenY);

/**
 * a simple integer division routine
 * doesn't do a remainder or anything
 */
int divSmall(int value, int divBy);

/**
 * functions for handling running and exiting the main loop
 */
int shouldContinueMainLoop();
void exitMainLoop();
void startMainLoop();





#endif /*MISC_H_*/
