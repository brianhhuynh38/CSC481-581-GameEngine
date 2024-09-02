#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "global.h"
#include "structs.h"
#include "draw.h"


namespace Render {

	/**
	* Defines and prepares the scene to render the next frame.
	* This was referenced from the SDL2 wiki:
	* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
	*/
	void prepareScene(void) {
		// Sets the rendering color for the window to an opaque blue
		SDL_SetRenderDrawColor(display->renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		// Color in entire rendering target specified on the previous line
		SDL_RenderClear(display->renderer);
	}

	/**
	* Renders the scene to the user
	* This was referenced from the SDL2 wiki:
	* https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
	*/
	void presentScene(void) {
		// Updates screen with anything that needs to be renderered
		SDL_RenderPresent(display->renderer);
	}

	/**
	 * Loads image at given filename.
	 * This was referenced from the SDL2 wiki:
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php
	 */
	SDL_Texture* loadTexture(const char* filename) {
		SDL_Texture* texture;
		// Logs loading percents when loading in the given file
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
		// Loads texture into the renderer
		texture = IMG_LoadTexture(display->renderer, filename);
		return texture;
	}

	/**
	 * Displays given texture at the given coordinates.
	 * This was referenced from the SDL2 wiki:
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php
	 */
	void displayTexture(SDL_Texture* texture, int x, int y) {
		// Destination and size of texture render
		SDL_Rect dest;
		// Set destination coordinates
		dest.x = x;
		dest.y = y;
		// Defines texture properties with the dimensions of the source texture
		// TODO: This can also be used to scale textures, look into this for section 6
		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		// Renders copies of the texture to the current target
		SDL_RenderCopy(display->renderer, texture, NULL, &dest);
	}
}
