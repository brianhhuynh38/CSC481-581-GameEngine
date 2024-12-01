#pragma once

#ifndef INPUT_H
#define INPUT_H

#include "definitions.h"

#include <map>


// Bytes that represent the different directions that are possible to be inputted
constexpr uint8_t INPUT_NONE = 0b00000000;
constexpr uint8_t INPUT_UP = 0b00001000;
constexpr uint8_t INPUT_DOWN = 0b00000100;
constexpr uint8_t INPUT_RIGHT = 0b00000010;
constexpr uint8_t INPUT_LEFT = 0b00000001;

// Bytes representing other inputs
constexpr uint8_t INPUT_ACTION = 0b10000000;
constexpr uint8_t INPUT_START_RECORDING = 0b01000000;
constexpr uint8_t INPUT_STOP_RECORDING = 0b00100000;
constexpr uint8_t INPUT_PLAYBACK_RECORDING = 0b00010000;


// Arrays that contain all special inputs
const uint8_t INPUT_TELEPORT[3] = { INPUT_DOWN, INPUT_UP, INPUT_DOWN };

// All types of special inputs that can be performed
enum IFlag {
	IFLAG_NONE = 0,
	IFLAG_TELEPORT = 1,
	IFLAG_START_REC = 2,
	IFLAG_STOP_REC = 3,
	IFLAG_PLAYBACK_REC = 4
};

/**
 * InputHandler holds the current input values for keypresses.
 */
typedef struct {
	// List of all available keys and their states
	int keyboard[MAX_KEYBOARD_KEYS];
	// The byte representing a combination of inputs
	uint8_t inputByte;
} InputHandler;


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
};


#endif
