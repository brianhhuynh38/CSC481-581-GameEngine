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

	// Update directional input byte
	keyboardState[SDL_SCANCODE_RIGHT] ? inputHandler->inputByte |= INPUT_RIGHT : inputHandler->inputByte &= ~INPUT_RIGHT;
	keyboardState[SDL_SCANCODE_LEFT] ? inputHandler->inputByte |= INPUT_LEFT : inputHandler->inputByte &= ~INPUT_LEFT;
	keyboardState[SDL_SCANCODE_UP] ? inputHandler->inputByte |= INPUT_UP : inputHandler->inputByte &= ~INPUT_UP;
	keyboardState[SDL_SCANCODE_DOWN] ? inputHandler->inputByte |= INPUT_DOWN : inputHandler->inputByte &= ~INPUT_DOWN;

	//// checks directional inputs (arrow keys)
	//checkKeyState(SDL_SCANCODE_RIGHT, keyboardState);
	//checkKeyState(SDL_SCANCODE_LEFT, keyboardState);
	//checkKeyState(SDL_SCANCODE_DOWN, keyboardState);
	//checkKeyState(SDL_SCANCODE_UP, keyboardState);

	//// checks interact input (E)
	//checkKeyState(SDL_SCANCODE_E, keyboardState);

	//// checks change to scaling/constant size input (-)
	//checkKeyState(SDL_SCANCODE_MINUS, keyboardState);
	//// checks pause and unpause
	//checkKeyState(SDL_SCANCODE_P, keyboardState);
	//// check for timescale changes (0.5)
	//checkKeyState(SDL_SCANCODE_8, keyboardState);
	//checkKeyState(SDL_SCANCODE_9, keyboardState);
	//checkKeyState(SDL_SCANCODE_0, keyboardState);
}

///**
//* Checks the state of the given keyboard key by referencing 
//* the keyboardState array and the inputHandler.
//* inputHander is then updated with the keys current state:
//* 0 = key is not pressed
//* 1 = key is currently pressed
//* 2 = key was just unpressed
//* @param scanCode key scanCode to check for
//* @param keyboardState array of keyboard key states
//*/
//void Input::checkKeyState(int scanCode, const Uint8* keyboardState) {
//	// if key is currently being pressed
//	if (keyboardState[scanCode]) {
//		inputHandler->keyboard[scanCode] = 1;
//		//inputHandler->inputByte |= INPUT
//	}
//	// if key is not currently being pressed, but previously was
//	else if (inputHandler->keyboard[scanCode] == 1) {
//		inputHandler->keyboard[scanCode] = 2;
//	}
//	// if key is not currently being pressed
//	else {
//		inputHandler->keyboard[scanCode] = 0;
//	}
//}
