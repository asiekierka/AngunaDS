#ifndef INPUT_H_
#define INPUT_H_

#include <nds.h>

/**
 * Scans the initial key state for checking key presses...
 * If any keys are down initially, they must be released and
 * re-pressed to count again for other waitKeyPress functions
 */
void updateKeyState();

/**
 * Waits for a keypress, or a specified number of frames
 */
void waitKeyPressOrTimeout(int timeout);

/**
 * Waits for a keypress before continuing
 */
void waitKeyPress();

/**
 * Checks whether the specified key is being held down
 */
bool isKeyDown(KEYPAD_BITS key);

/**
 * Checks whether the specified key is "pressed" ...
 * The key triggers true when the button is first pressed, but
 * only triggers once, and must be released before it can be 
 * pressed again. 
 */
bool isKeyPress(KEYPAD_BITS key);

/**
 * Whether any of the button (non directional) keys has been pressed
 */
bool isButtonKeyPressed();


#endif /*INPUT_H_*/
