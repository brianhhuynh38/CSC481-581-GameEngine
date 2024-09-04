#pragma once
#ifndef DRAW_H
#define DRAW_H

namespace Render {

	/**
	 * Defines and prepares the scene to render the next frame.
	 * This was referenced from the SDL2 wiki:
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
	 */
	void prepareScene(void);

	/**
	 * Renders the scene to the user
	 * This was referenced from the SDL2 wiki:
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
	 */
	void presentScene(void);

	/**
	 * Loads image at given filename.
	 * This was referenced from the SDL2 wiki:
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php
	 * @param filename for texture to load
	 * @return texture in SDL_Texture format
	 */
	SDL_Texture* loadTexture(const char* filename);

	/**
	 * Displays given texture at the given coordinates.
	 * This was referenced from the SDL2 wiki:
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php
	 * @param texture to display
	 * @param x coordinate of texture
	 * @param y coordinate of texture
	 * @param w width scale of texture
	 * @param h height scale of texture
	 */
	void displayTexture(SDL_Texture* texture, float x, float y, float w, float h);

}

#endif
