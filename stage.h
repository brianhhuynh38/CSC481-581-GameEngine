#include <SDL.h>
#include <SDL_image.h>
#include "global.h"
#include "structs.h"
#include "definitions.h"
#include "entity.h"
#include "player.h"
#include "movingEntity.h"

#pragma once
#ifndef STAGE_H
#define STAGE_H

namespace Stages {
	class Stage {
	protected:
		/** list of entities in the stage */
		//Entities::Entity entities[];

		/** list of players in the stage */
		Entities::Player players[];
	public:
		/**
		 * Initiates the stage 
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		void initStage(void);

		/**
		 * Initiates the player entity.
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void initPlayer(void);
	
		/**
		 * Runs the stages logic.
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void logic(void);

		/**
		 * Draws the stage and it's entities.
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void draw(void);

		/**
		 * Caps the framerate
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void capFrameRate(long* then, float* remainder);


	};
}
#endif