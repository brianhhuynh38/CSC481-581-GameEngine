#pragma once
#include "structs.h"
#ifndef INPUT_H
#define INPUT_H


// Bytes that represent the different directions that are possible to be inputted
constexpr int8_t INPUT_UP = 0x1000;
constexpr int8_t INPUT_DOWN = 0x0100;
constexpr int8_t INPUT_RIGHT = 0x0010;
constexpr int8_t INPUT_LEFT = 0x0001;


class Input {
private:
	/// The InputHandler struct used to keep track of keypresses and other input.
	InputHandler *inputHandler;
public:
	Input(InputHandler *inputHandler);
	/**
	* Takes inputs from the user
	* This was referenced from the SDL2 wiki:
	* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
	*/
	void takeInput();

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
	void checkKeyState(int scanCode, const Uint8* keyboardState);
};

#endif
