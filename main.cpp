#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "definitions.h"
#include "draw.h"
#include "input.h"
#include "global.h"
#include "structs.h"
#include "entity.h"
#include "stage.h"
#include "player.h"
#include "configIO.h"
#include "playerController.h"
#include "entityController.h"

// Global variables
/// The Display struct used to initialize renderer and window
Display *display;
/// The entity that the player is able to control
Entities::Player *player;
/// Bullet entity (TEST)
Entities::Entity *ball;
/// floor entity (TEST)
Entities::Entity *ground;
/// The default player controller
Controllers::PlayerController *playerController;
/// Controller for all entities and their physics
EntityController* entityController;
/// The InputHandler struct used to keep track of keypresses and other input.
InputHandler inputHandler;
/// The physics system
Physics physics;

Timeline timeline;

/**
 * Frees any allocated memory on application exit
 */
void memoryCleanUp() {
	delete display;
	// Free memory for player
	player->destroy();
	delete player;
}

/**
 * The function meant to initialize the SDL window and renderer. This part of the code was created
 * using the tutorial linked to on the SDL Wiki here:
 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
 *
 */
void initSDL(void) {
	// Define SDL rendering flags
	int rendererFlags, windowFlags;
	// Renderer uses hardware acceleration
	rendererFlags = SDL_RENDERER_ACCELERATED;
	// Window does not use any specialized flags
	windowFlags = 0;

	// If negative error code, exit with failure status
	if (int status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cout << "SDL failed to start up:" << status;
		exit(1);
	}

	// Create window that is centered
	// TODO: Might change later to not automatically use default resolution and have it adjust
	display->window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		DEFAULT_WIDTH, DEFAULT_HEIGHT, windowFlags);
	// Exit if the window fails to open
	if (!display->window) {
		std::cout << "The window was unable to open: " << SDL_GetError();
		exit(1);
	}

	// Optional configuration that makes rendered objects linearly interpolate with each other
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	// Create renderer, -1 index so that it looks for the first hardware accelerator it can find
	display->renderer = SDL_CreateRenderer(display->window, -1, rendererFlags);
	// Exit if the renderer fails to be created
	if (!display->renderer) {
		std::cout << "The renderer failed to be created: " << SDL_GetError();
	}
	// Allows the use of JPG and PNG files by initializing them in SDL
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
}

/**
 * Caps the framerate
 * This references the tutorial linked to on the SDL wiki here :
 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
 */
static void capFrameRate(long* then, float* remainder) {
	long wait, frameTime;

	wait = 16 + *remainder;

	*remainder -= (int)*remainder;

	frameTime = SDL_GetTicks() - *then;

	wait -= frameTime;

	if (wait < 1) {
		wait = 1;
	}

	SDL_Delay(wait);

	*remainder += 0.667;

	*then = SDL_GetTicks();
}

/**
 * The main function where all components are initialized and the gameplay update loop is maintained 
 */
int main(int argc, char* argv[]) {
	
	long then;
	float remainder;
	
	// Allocate memory to SDL renderer components and instantiate Display struct
	display = new Display;

	//inputHandler = new InputHandler;
	
	// Initialize SDL components
	initSDL();

	// Runs memoryCleanUp() if the application exits
	std::atexit(memoryCleanUp);

	// Initiate current stage
	//initStage();

	then = SDL_GetTicks();

	remainder = 0;

	

	// Create a player Entity (Temp: Make more malleable in the future)
	// TODO: Base starting position off window size percentage
	player = new Entities::Player(
		1.0, 1.0,
		250.0, 250.0,
		10.0,
		"./Assets/Textures/DefaultPlayerTexture1.png",
		false,
		true,
		1.0
	);
	// Create temporary ball object
	ball = new Entities::Entity(
		1.0, 1.0,
		550.0, 250.0,
		10.0,
		"./Assets/Textures/BallTexture.png",
		false,
		true
	);
	// Create temporary box object that moves
	/*ball = new Entities::MovingEntity(
		1.0, 1.0,
		550.0, 550.0,
		10.0,
		"./Assets/Textures/devTexture0.png",
		false,
		true,
	);*/
	// Create temporary ground object
	ground = new Entities::Entity(
		20.0, 20.0,
		250.0, 550.0,
		10.0,
		"./Assets/Textures/devTexture1.png",
		true,
		false
	);

	// Create player controller for player (temp for testing)
	playerController = new Controllers::PlayerController(player);
	//Create entity controller
	entityController = new EntityController();
	//entityController->addEntity(*player);
	//entityController->addEntity(*ground);
	//entityController->addEntity(*ball);

	// Create Timeline
	timeline = Timeline();
	// Create physics
	physics = Physics(); 


	// Loads in config file to read and get configured gravity
	loadConfigFile();

	// Basic, primitive game loop
	// TODO: Add ability to reload everything via terminal at some point
	while (true) {
		// Updates to get a new deltaTime
		timeline.updateTime();

		// Prepares scene for rendering
		Render::prepareScene();

		// Updates the keyboard inputs
		SDL_PumpEvents();
		// Handles player input, including exit
		Input::takeInput();

		// Run logic and draw scene
		//display->delegate.logic();
	    //display->delegate.draw();

		// check player movmentInput (TESTING)
		playerController->movementInput();

		// Display player and floor texture at their locations
		Render::displayTexture(player->getTexture(), player->getPosition()->x, player->getPosition()->y, player->getScale()->x, player->getScale()->y);
		Render::displayTexture(ground->getTexture(), ground->getPosition()->x, ground->getPosition()->y, ground->getScale()->x, ground->getScale()->y);
		Render::displayTexture(ball->getTexture(), ball->getPosition()->x, ball->getPosition()->y, ball->getScale()->x, ball->getScale()->y);
		
		// Renders the scene gven the parameters identified in prepareScene()
		Render::presentScene();

		// Update the physics of all entities
		//entityController->updateEntities();

		// Slow down by 16 ms to maintain approximately 62 fps
		SDL_Delay(16);

		capFrameRate(&then, &remainder);
	}

	// Success condition
	return 0;
}

