#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "definitions.h"
#include "draw.h"
#include "input.h"
#include "global.h"
#include "structs.h"
#include "entity.h"

// Global variables
/// The Display struct used to initialize renderer and window
Display *display;

/// The player
Entity *player;

/**
* Frees any allocated memory on application exit
*/
void memoryCleanUp() {
	free(display);
	free(player);
}

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
	// Runs memoryCleanUp() if the application exits
	std::atexit(memoryCleanUp);

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

	// Initializes SDL Image. Allows loading of PNGs and JPGs.
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
}

/**
* The main function where all components are initialized and the gameplay update loop is maintained 
*/
int main(int argc, char* argv[]) {
	//// Temporary setup message, delete later
	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Hello", "It works", NULL);
	// Allocate memory to SDL renderer components and instantiate Display struct
	memset(&display, 0, sizeof(Display));
	memset(&player, 0, sizeof(Entity));

	// Initialize SDL components
	initSDL();

	display = new Display;
	player = new Entity(Vector{ 1, 1 }, Vector{ 1, 1 }, 0, Vector{ 0, 0 }, loadTexture((char*)"assets/gfx/player.png"));

	// Basic, primitive game loop
	while (true) {
		// Prepares scene for rendering
		prepareScene(display->renderer);
		// Updates the keyboard inputs
		SDL_PumpEvents();
		// Handles player input, including exit
		takeInput();
		// Display player texture at player location
		blit(player->getTexture(), player->getPosition().x, player->getPosition().y);
		// Renders the scene gven the parameters identified in prepareScene()
		presentScene(display->renderer);
		// Slow down by 16 ms to maintain approximately 62 fps
		SDL_Delay(16);
	}

	// Success condition
	return 0;
}

