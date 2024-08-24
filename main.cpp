#include <SDL.h>
#include "structs.h"
#include "definitions.h"
#include <iostream>

// Global variables
/// The Display struct used to initialize renderer and window
Display *display;

/**
 * The function meant to initialize the SDL window and renderer. This part of the code was created
 * using the tutorial linked to on the SDL Wiki here:
 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
 */
void initSDL(void) {
	// Define SDL rendering flags
	int rendererFlags, windowFlags;
	// Renderer uses hardware acceleration
	rendererFlags = SDL_RENDERER_ACCELERATED;
	// Window does not use any specialized flags
	windowFlags = 0;
	// If negative error code, exit with failure status
	if (int status = SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL failed to start up:" << status;
		exit(1);
	}
	// Create window that is centered
	// TODO: Might change later to not automatically use default resolution and have it adjust
	display->window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		DEFAULT_WIDTH, DEFAULT_HEIGHT, windowFlags);
	// Exit if the window fails to open
	if (!display->window) {
		std::cout << "The window was unable to open: " << SDL_GetError();
		exit(1);
	}
	// Create renderer, -1 index so that it looks for the first hardware accelerator it can find
	display->renderer = SDL_CreateRenderer(display->window, -1, rendererFlags);
	// Exit if the renderer fails to be created
	if (!display->renderer) {
		std::cout << "The renderer failed to be created: " << SDL_GetError();
	}
}

int main(int argc, char* argv[]) {
	// Temporary setup message, delete later
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello", "It works", NULL);
	// Allocate memory to SDL renderer components
	display = (Display*)malloc(sizeof(Display*));
	display->renderer = (SDL_Renderer*)malloc(sizeof(SDL_Renderer*));
	display->window = (SDL_Window*)malloc(sizeof(SDL_Window*));
	// Initialize SDL components
	initSDL();
	// Success condition
	return 0;
}
