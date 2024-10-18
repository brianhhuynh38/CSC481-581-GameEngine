#include <list>

#include "stage.h"
#include "draw.h"

namespace Stages {

	/**
	 * Draws the players in the stage.
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	void Stage::drawPlayers(void) {

		//// Display player texture at player location
		//Render::displayTexture(
		//	player->getTexture(),
		//	player->getPosition()->x,
		//	player->getPosition()->y,
		//	player->getSize()->x,
		//	player->getSize()->y,
		//	player->getScale()->x, 
		//	player->getScale()->y
		//);
		//
	}

	/**
	 * Draws the entities in the stage.
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	void Stage::drawEntities(void) {
		//// Create entity list iterator
		//std::list<Entities::Entity>::iterator iter;
		//std::list<Entities::Entity> entities = *entityController->getEntities();
		//// Draws each entity
		//for (iter = entities.begin(); iter != entities.end(); ++iter) {
		//	Render::displayTexture(iter->getTexture(), iter->getPosition()->x, iter->getPosition()->y, iter->getSize()->x, iter->getSize()->y, iter->getScale()->x, iter->getScale()->y);
		//}
	}

	/**
	 * Draws the stage and it's entities.
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	void Stage::draw(void) {
		drawPlayers();

		drawEntities();
	}

	/**
	 * Runs player logic
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	void Stage::doPlayers(void) {
		
	}

	/**
	 * Runs entity logic.
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	void Stage::doEntities(void) {

		//entityController->updateEntities();
		
	}

	/**
	 * Runs the stages logic.
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	void Stage::logic(void) {
		// runs player logic
		doPlayers();
		// runs entity logic
		doEntities();
	}

	/**
	 * Initiates the stage
	 * using the tutorial linked to on the SDL Wiki here:
     * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	void Stage::initStage(void) {
		display->delegate.logic = logic;
		display->delegate.draw = draw;

		//stage = new Stage;
		//memset(&stage, 0, sizeof(stage));


		initPlayer();
		
	}

	/**
	 * Initiates the player entity.
	 * This references the tutorial linked to on the SDL wiki here :
	 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
	 */
	void Stage::initPlayer(void) {
		//player = new Entities::Player(
		//	1.0, 1.0,
		//	250.0, 250.0,
		//	15.0, 25.0,
		//	10.0,
		//	"./Assets/Textures/DefaultPlayerTexture1.png",
		//	false,
		//	true,
		//	0.0f, -50.0f,
		//	1.0
		//);
		////m_players.insert();

		//player->setTexture( (char*) "./Assets/Textures/DefaultPlayerTexture1.png");
		//Render::displayTexture(player->getTexture(), player->getPosition()->x, player->getPosition()->y, player->getSize()->x, player->getSize()->y, player->getScale()->x, player->getScale()->y);
		//
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