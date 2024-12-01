#include <SDL.h>
#include <iostream>

#include "input.h"
#include "structs.h"
#include "global.h"


Input::Input(InputHandler *inputHandler) {
	this->inputHandler = inputHandler;
}

/**
* Takes inputs from the user
* This was referenced from the SDL2 wiki:
* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
*/
void Input::takeInput() {
	/// <summary>
	/// An array that returns a value of 0 or 1 for each key on the keyboard
	/// depending on whether or not the key is pressed (1 is pressed)
	/// </summary>
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	// Quits the application if the Escape key is pressed
	if (keyboardState[SDL_SCANCODE_ESCAPE]) {
		exit(0);
	}

	// Update directional input bits
	keyboardState[SDL_SCANCODE_RIGHT] ? inputHandler->inputByte |= INPUT_RIGHT : inputHandler->inputByte &= ~INPUT_RIGHT;
	keyboardState[SDL_SCANCODE_LEFT] ? inputHandler->inputByte |= INPUT_LEFT : inputHandler->inputByte &= ~INPUT_LEFT;
	keyboardState[SDL_SCANCODE_UP] ? inputHandler->inputByte |= INPUT_UP : inputHandler->inputByte &= ~INPUT_UP;
	keyboardState[SDL_SCANCODE_DOWN] ? inputHandler->inputByte |= INPUT_DOWN : inputHandler->inputByte &= ~INPUT_DOWN;

	// Update interact button
	keyboardState[SDL_SCANCODE_E] ? inputHandler->inputByte |= INPUT_ACTION : inputHandler->inputByte &= ~INPUT_ACTION;

	// Update playback controls
	keyboardState[SDL_SCANCODE_LEFTBRACKET] ? inputHandler->inputByte |= INPUT_START_RECORDING : inputHandler->inputByte &= ~INPUT_START_RECORDING;
	keyboardState[SDL_SCANCODE_RIGHTBRACKET] ? inputHandler->inputByte |= INPUT_STOP_RECORDING : inputHandler->inputByte &= ~INPUT_STOP_RECORDING;
	keyboardState[SDL_SCANCODE_P] ? inputHandler->inputByte |= INPUT_PLAYBACK_RECORDING : inputHandler->inputByte &= ~INPUT_PLAYBACK_RECORDING;


	//// checks change to scaling/constant size input (-)
	//checkKeyState(SDL_SCANCODE_MINUS, keyboardState);
}
