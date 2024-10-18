#pragma once

#ifndef DRAW_H
#define DRAW_H

#include "entity.h"
#include "GameObject.h"

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
	SDL_Texture* loadTexture(std::string filename);

	/**
	 * Displays given texture at the given coordinates.
	 * This was referenced from the SDL2 wiki:
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php
	 * @param texture to display
	 * @param x coordinate of texture
	 * @param y coordinate of texture
	 * @param w width of texture
	 * @param h height of texture
	 * @param sw width scale of texture
	 * @param sh height scale of texture
	 */
	void displayTexture(SDL_Texture* texture, float x, float y, float w, float h, float sh, float sw);

	/**
	 * Displays the given entities texture
	 * (just a short version of the displayTexture function)
	 * @param entity to display
	 */
	void displayEntity(Entities::Entity entity);

	/**
	 * Displays the given GO texture
	 * (just a short version of the displayTexture function)
	 * @param entity to display
	 */
	void displayGameObject(GameObject& gameObject);

}
#endif
