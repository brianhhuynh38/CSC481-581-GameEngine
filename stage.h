#include <SDL.h>
#include <SDL_image.h>
#include "global.h"
#include "structs.h"
#include "definitions.h"
#include "entity.h"
#include "player.h"
#include "movingEntity.h"
#include <list>

#pragma once
#ifndef STAGE_H
#define STAGE_H

namespace Stages {
	class Stage {
	protected:
		//int map[MAP_WIDTH][MAP_HEIGHT];
		static std::list<Entities::Player> m_players;
		static std::list<Entities::Entity> m_entities;
	public:

		/**
		 * Default stage constructor 
		 */
		Stage();

		/**
		 * Draws the players in the stage.
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void drawPlayers(void);

		/**
		 * Draws the entities in the stage.
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void drawEntities(void);

		/**
		 * Draws the stage and it's entities.
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void draw(void);

		/**
		 * Runs player logic
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void doPlayers(void);

		/**
		 * Runs entity logic.
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void doEntities(void);

		/**
		 * Runs the stages logic.
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void logic(void);

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
		 * Caps the framerate
		 * This references the tutorial linked to on the SDL wiki here :
		 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
		 */
		static void capFrameRate(long* then, float* remainder);


	};
}
#endif