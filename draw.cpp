#include <SDL.h>
#include "structs.h"
#include <iostream>

#include "draw.h"

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
