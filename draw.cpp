#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "draw.h"
#include "global.h"

/**
* Defines and prepares the scene to render the next frame.
* This was referenced from the SDL2 wiki:
* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
*/
void prepareScene(SDL_Renderer* renderer) {
	// Sets the rendering color for the window to an opaque blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	// Color in entire rendering target specified on the previous line
	SDL_RenderClear(renderer);
}

/**
* Renders the scene to the user
* This was referenced from the SDL2 wiki:
* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
*/
void presentScene(SDL_Renderer* renderer) {
	// Updates screen with anything that needs to be renderered
	SDL_RenderPresent(renderer);
}

/**
 * Loads image at given filename.
 * This was referenced from the SDL2 wiki:
 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php
 */
SDL_Texture *loadTexture(char *filename) {
	SDL_Texture *texture;

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
	
	texture = IMG_LoadTexture(display->renderer, filename);

	return texture;
}

/**
 * Displays given texture at the given coordinates.
 * This was referenced from the SDL2 wiki:
 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php
 */
void blit(SDL_Texture *texture, int x, int y) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(display->renderer, texture, NULL, &dest);
}