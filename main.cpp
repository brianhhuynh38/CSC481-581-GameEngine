#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

using namespace GEngine::Render;
using namespace GEngine::Input;

#include "definitions.h"
#include "draw.h"
#include "input.h"
#include "global.h"
#include "structs.h"

// Global variables
/// The Display struct used to initialize renderer and window
Display *display;

/**
* Frees any allocated memory on application exit
*/
void memoryCleanUp() {
	free(display);
}

/**
* The function meant to initialize the SDL window and renderer. This part of the code was created
* using the tutorial linked to on the SDL Wiki here:
* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
*
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

	// Optional configuration that makes rendered objects linearly interpolate with each other
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	// Create renderer, -1 index so that it looks for the first hardware accelerator it can find
	display->renderer = SDL_CreateRenderer(display->window, -1, rendererFlags);
	// Exit if the renderer fails to be created
	if (!display->renderer) {
		std::cout << "The renderer failed to be created: " << SDL_GetError();
	}
	// Allows the use of JPG and PNG files
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
}

/**
* The main function where all components are initialized and the gameplay update loop is maintained 
*/
int main(int argc, char* argv[]) {
	// Allocate memory to SDL renderer components and instantiate Display struct
	memset(&display, 0, sizeof(Display));
	display = new Display;

	//memset(&player, 0, sizeof(Entity);) 
	// 
	// Runs memoryCleanUp() if the application exits
	std::atexit(memoryCleanUp);

	// Initialize SDL components
	initSDL();

	// starting player variables
	//player.x = 100;
	//player.y = 100;
	//player.texture = loadTexture("");

	// Basic, primitive game loop
	while (true) {
		// Prepares scene for rendering
		prepareScene();
		// Updates the keyboard inputs
		SDL_PumpEvents();
		// Handles player input, including exit
		takeInput();
		// Display player texture at player location
		//blit(player.texture, player.x, player.y);
		// Renders the scene gven the parameters identified in prepareScene()
		presentScene();
		// Slow down by 16 ms to maintain approximately 62 fps
		SDL_Delay(16);
	}

	// Success condition
	return 0;
}

