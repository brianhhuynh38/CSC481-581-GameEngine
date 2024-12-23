#include "draw.h"

#include <SDL_image.h>
#include <iostream>
#include <cassert>
#include <string>

#include "global.h"
#include "structs.h"

#include "transform.h"
#include "textureMesh.h"

namespace Render {

    /**
	 * Defines and prepares the scene to render the next frame.
	 * This was referenced from the SDL2 wiki:
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
	 */
	void prepareScene(void) {
		// Sets the rendering color for the window to an opaque blue
		SDL_SetRenderDrawColor(display->renderer, 68, 149, 131, SDL_ALPHA_OPAQUE);
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
		if (proportionalScalingActive) {
			// Renders given the global scale
			SDL_RenderSetScale(display->renderer, globalScaling.x, globalScaling.y);
		}
		else {
			// Renders given constant scale
			SDL_RenderSetScale(display->renderer, 1, 1);
		}
	}

	/**
	 * Loads image at given filename.
	 * This was referenced from the SDL2 wiki:
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter2.php
	 * @param filename for texture to load
	 * @return texture in SDL_Texture format
	 */
	SDL_Texture* loadTexture(std::string filename) {
		SDL_Texture* texture;
		// Logs loading percents when loading in the given file
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename.c_str());

		//std::cout << "FileName: " << filename << "\n";
		//assert(filename == "./Assets/Textures/DefaultPlayerTexture1.png");

		// Tries to load in texture
		try {
			// Loads texture into the renderer
			texture = IMG_LoadTexture(display->renderer, filename.c_str());
		}
		catch (int e) {
			// If the filename cannot be found, or is corrupted, load in missing texture instead
			std::cout << "Failed to load texture: " << filename << "\nLoading missing texture instead...";
			texture = IMG_LoadTexture(display->renderer, "./Assets/Textures/MissingTexture.png");
		}
		
		return texture;
	}

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
	void displayTexture(SDL_Texture* texture, float x, float y, float w, float h, float sw, float sh) {
		// Destination and size of texture render
		SDL_Rect dest;
	
		// Set destination coordinates
		dest.x = x;
		dest.y = y;
		dest.w = sw;
		dest.h = sh;

		// Defines texture properties with the dimensions of the source texture
		// TODO: This can also be used to scale textures, look into this for section 6
		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		// Renders copies of the texture to the current target
		SDL_RenderCopy(display->renderer, texture, NULL, &dest);
	}

	/**
	* Displays the given entities texture
	* (just a short version of the displayTexture function)
	* @param entity to display
	*/
	void displayGameObject(GameObject& gameObject, Utils::Vector2D cameraPos) {
		// Get necessary components
		Components::TextureMesh *tm = gameObject.getComponent<Components::TextureMesh>();
		Components::Transform *transform = gameObject.getComponent<Components::Transform>();

		// If null, do not display
		if (tm == nullptr || transform == nullptr) {
			return;
		}

		// Displays the texture given the necessary components
		displayTexture(tm->getTexture(), transform->getPosition()->x - cameraPos.x, transform->getPosition()->y - cameraPos.y,
			transform->getSize().x, transform->getSize().y, transform->getScale().x, transform->getScale().y);

		//std::cout << "Destination Rendered: " << transform->getPosition()->x << ", " << transform->getPosition()->y << "\n";
	}

}
