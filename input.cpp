#include <SDL.h>

#include "input.h"

/// <summary>
/// An array that returns a value of 0 or 1 for each key on the keyboard
/// depending on whether or not the key is pressed (1 is pressed)
///</summary>
const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

/**
* Takes inputs from the user
* This was referenced from the SDL2 wiki:
* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
*/
void takeInput() {
	// Quits the application if the Escape key is pressed
	if (keyboardState[SDL_SCANCODE_ESCAPE] > 0) {
		exit(0);
	}
}
