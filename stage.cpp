#include "stage.h"

namespace Stages {

	/**
	 * Runs the stages logic.
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	static void logic(void) {

	}

	/**
	 * Draws the stage and it's entities.
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	static void draw(void) {

	}

	/**
	 * Initiates the stage
	 * using the tutorial linked to on the SDL Wiki here:
     * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	void initStage(void) {
		display->delegate.logic = logic;
		display->delegate.draw = draw;

		//stage = new Stage;
		
	}

	/**
	 * Initiates the player entity.
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	static void initPlayer(void) {
		/*player = malloc(sizeof(Entities::Player));
		memset(player, 0, sizeof(Entities::Entity));
		stage.fighterTail->next = player;
		stage.fighterTail = player;

		player->x = 100;
		player->y = 100;
		player->texture = loadTexture("gfx/player.png");
		SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
		*/
	} 

	/**
	 * Caps the framerate
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	static void capFrameRate(long* then, float* remainder) {
		long wait, frameTime;

		wait = 16 + *remainder;

		*remainder -= (int)* remainder;

		wait -= frameTime;

		if (wait < 1) {
			wait = 1;
		}

		SDL_Delay(wait);

		*remainder += 0.667;

		*then = SDL_GetTicks();
	}
}