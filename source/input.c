#include "input.h"
#include "misc.h"



bool isButtonKeyPressed()
{
	return isKeyPress(KEY_A) || 
		isKeyPress(KEY_B) ||
		isKeyPress(KEY_X) ||
		isKeyPress(KEY_Y) ||
		isKeyPress(KEY_L) ||
		isKeyPress(KEY_R) ||
		isKeyPress(KEY_SELECT) ||
		isKeyPress(KEY_START) ||
		isKeyPress(KEY_TOUCH);
}

/**
 * Scans the initial key state for checking key presses...
 * If any keys are down initially, they must be released and
 * re-pressed to count again for other waitKeyPress functions
 */
void updateKeyState()
{
}

/**
 * Waits for a keypress, or a specified number of frames
 */
void waitKeyPressOrTimeout(int timeout)
{
	int ticks = 0;
	bool done = false;
	while (!done)
	{
		frameTick();
		done = isButtonKeyPressed();
		ticks++;
		if (ticks >= timeout)
		{
			done = true;
		}
	}
}

/**
 * Waits for a keypress before continuing
 */
void waitKeyPress()
{
	bool done = false;
	while (!done)
	{
		frameTick();
		done = isButtonKeyPressed();
	}
}

/**
 * Checks whether the specified key is being held down
 */
bool isKeyDown(KEYPAD_BITS key)
{
	uint32 keys = keysHeld();
	return (keys & key) != 0;
}

/**
 * Checks whether the specified key is "pressed" ...
 * The key triggers true when the button is first pressed, but
 * only triggers once, and must be released before it can be 
 * pressed again
 */
bool isKeyPress(KEYPAD_BITS key)
{
	uint32 keys = keysDown();
	return (keys & key) != 0;
}

