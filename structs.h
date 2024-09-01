#include <SDL.h>
#include "definitions.h"
#pragma once

/*
* Display contains pointer instances to SDL_Window and SDL_Renderer.
* This references the tutorial linked to on the SDL wiki here:
* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
*/
typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
} Display;

/**
* 
*/
typedef struct {
	int map[MAP_WIDTH][MAP_HEIGHT];
} Stage;

/**
* Coordinate system to use for position-based
*/
typedef struct {
	int x;
	int y;
} Vector;
