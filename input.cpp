#include <SDL.h>

#include "input.h"
#include "structs.h"
#include "global.h"

namespace Input {
	/**
	* Takes inputs from the user
	* This was referenced from the SDL2 wiki:
	* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
	*/
	void takeInput() {
		/// <summary>
		/// An array that returns a value of 0 or 1 for each key on the keyboard
		/// depending on whether or not the key is pressed (1 is pressed)
		/// </summary>
		const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
		
		// Quits the application if the Escape key is pressed
		if (keyboardState[SDL_SCANCODE_ESCAPE]) {
			exit(0);
		}

		// Checks if right key is pressed
		if (keyboardState[SDL_SCANCODE_RIGHT]) {
			inputHandler.right = 1;
		}
		// Checks if right key was un-pressed
		else if (inputHandler.right == 1) {
			inputHandler.right = 0;
		}

		//Checks if left key is pressed
		if (keyboardState[SDL_SCANCODE_LEFT]) {
			inputHandler.left = 1;
		}
		// Checks if left key was un-pressed
		else if (inputHandler.left == 1) {
			inputHandler.left = 0;
		}

		//Checks if down key is pressed
		if (keyboardState[SDL_SCANCODE_DOWN]) {
			inputHandler.down = 1;
		}
		// Checks if down key was un-pressed
		else if (inputHandler.down == 1) {
			inputHandler.down = 0;
		}

		//Checks if up key is pressed
		if (keyboardState[SDL_SCANCODE_UP]) {
			inputHandler.up = 1;
		}
		// Checks if up key was un-pressed
		else if (inputHandler.up == 1) {
			inputHandler.up = 0;
		}
	}

}
