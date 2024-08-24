#include <SDL.h>
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
