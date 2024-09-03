#include <SDL.h>
#include <iostream>

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

		// checks directional inputs (arrow keys)
		checkKeyState(SDL_SCANCODE_RIGHT, keyboardState);
		checkKeyState(SDL_SCANCODE_LEFT, keyboardState);
		checkKeyState(SDL_SCANCODE_DOWN, keyboardState);
		checkKeyState(SDL_SCANCODE_UP, keyboardState);

		// checks interact input (E)
		checkKeyState(SDL_SCANCODE_E, keyboardState);
		
	}

	/**
	 * Checks the state of the given keyboard key by referencing 
	 * the keyboardState array and the inputHandler.
	 * inputHander is then updated with the keys current state:
	 * 0 = key is not pressed
	 * 1 = key is currently pressed
	 * 2 = key was just unpressed
	 * @param scanCode key scanCode to check for
	 * @param keyboardState array of keyboard key states
	 */
	void checkKeyState(int scanCode, const Uint8* keyboardState) {
		// if key is currently being pressed
		if (keyboardState[scanCode]) {
			inputHandler.keyboard[scanCode] = 1;
		}
		// if key is not currently being pressed, but previously was
		else if (inputHandler.keyboard[scanCode] == 1) {
			inputHandler.keyboard[scanCode] = 2;
		}
		// if key is not currently being pressed
		else {
			inputHandler.keyboard[scanCode] = 0;
		}
	}

}
