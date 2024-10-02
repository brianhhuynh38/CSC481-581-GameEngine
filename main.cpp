#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <zmq.hpp>
#include <string>
#include <mutex>

#include "definitions.h"
#include "draw.h"
#include "input.h"
#include "global.h"
#include "structs.h"
#include "entity.h"
#include "stage.h"
#include "player.h"
#include "movingEntity.h"
#include "configIO.h"
#include "playerController.h"
#include "entityController.h"
#include "client.h"
#include "thread.h"

// Global variables
/// The Display struct used to initialize renderer and window
Display *display;
/// The entity that the player is able to control
Entities::Player *player;
/// Ball entity (TEST)
Entities::Entity *ball;
/// movingBox entity (TEST)
Entities::MovingEntity *movingBox;
/// floor entity (TEST)
Entities::Entity *ground;
/// platform entity (TEST)
Entities::Entity *platform;
/// The default player controller
Controllers::PlayerController *playerController;
/// Controller for all entities and their physics
EntityController* entityController;
/// The InputHandler struct used to keep track of keypresses and other input.
InputHandler inputHandler;

/// Global scale factor
Utils::Vector2D globalScaling;
/// Determines if proportional scaling is active
bool proportionalScalingActive;

/// The physics system
Physics physics;
/// The timeline used to keep track of time intervals
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
void initSDL(ConfigSettings settings) {
	// Define SDL rendering flags
	int rendererFlags, windowFlags;
	// Renderer uses hardware acceleration
	rendererFlags = SDL_RENDERER_ACCELERATED;
	// Window does not use any specialized flags
	windowFlags = 0;

	// If negative error code, exit with failure status
	if (int status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cout << "SDL failed to start up:" << status << "\n";
		exit(1);
	}

	// Create window that is centered
	// TODO: Might change later to not automatically use default resolution and have it adjust
	display->window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		settings.resolutionWidth, settings.resolutionHeight, windowFlags);
	// Exit if the window fails to open
	if (!display->window) {
		std::cout << "The window was unable to open: " << SDL_GetError() << "\n";
		exit(1);
	}

	// Optional configuration that makes rendered objects linearly interpolate with each other
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	// Create renderer, -1 index so that it looks for the first hardware accelerator it can find
	display->renderer = SDL_CreateRenderer(display->window, -1, rendererFlags);
	// Exit if the renderer fails to be created
	if (!display->renderer) {
		std::cout << "The renderer failed to be created: " << SDL_GetError() << "\n";
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

	wait = 16667 + *remainder;

	*remainder -= (int) *remainder;

	frameTime = timeline.getTime() - *then;

	wait -= frameTime;

	if (wait < 1000) {
		wait = 1000;
	}

	SDL_Delay(wait/1000);

	*remainder += 666.667;

	*then = timeline.getTime();

	//std::cout << *then;
}


/**
 * The main function where all components are initialized and the gameplay update loop is maintained 
 */
int main(int argc, char* argv[]) {
	
	long then;
	float remainder;
	
	// Allocate memory to SDL renderer components and instantiate Display struct
	display = new Display;

	// Create Timeline
	timeline = Timeline();
	// Create physics
	physics = Physics();

	// TODO: Initialize networking
	// initialize the zmq context with a single IO thread
	zmq::context_t context{ 2 };
	// construct a SUB (subscribe) socket to receive entity movements and checks from the server
	zmq::socket_t serverToClientSubscriber{ context, zmq::socket_type::sub };
	// construct a REQ (request) socket to receive client identification information from the server
	zmq::socket_t clientToServerRequest{ context, zmq::socket_type::req };
	// construct a PUB (publish) socket to send player information to the server
	zmq::socket_t clientToServerPublisher{ context, zmq::socket_type::pub };

	ConfigSettings settings = ConfigSettings();

	// Loads in config file to read and get configured gravity
	loadConfigFile(&settings);
	
	// Initialize SDL components
	initSDL(settings);

	// Runs memoryCleanUp() if the application exits
	std::atexit(memoryCleanUp);

	// Initiate current stage
	//initStage();

	then = timeline.getTime();
	remainder = 0;

	// Create a player Entity (Temp: Make more malleable in the future)
	// TODO: Base starting position off window size percentage
	
	entityController = new EntityController();
	// Update request and subscriber. Put on a new thread
	Client::startup(&serverToClientSubscriber, &clientToServerRequest, &clientToServerPublisher, player, entityController, playerController);


	ball = new Entities::Entity
	(
		1.0, 1.0,
		550.0, 250.0,
		20.0, 20.0,
		10.0,
		"./Assets/Textures/BallTexture.png",
		true,
		false
	);
	ball->setUUID(-2);
	// Create ground object (Temp)
	ground = new Entities::Entity(
		1.0, 1.0,
		250.0, 550.0,
		128.0, 64.0,
		10.0,
		"./Assets/Textures/devTexture1.png",
		true,
		false
	);
	ground->setUUID(-3);

	// Create second ground object (Temp)
	platform = new Entities::Entity(
		1.0, 1.0,
		350.0, 525.0,
		64.0, 64.0,
		10.0,
		"./Assets/Textures/devTexture3.png",
		true,
		false
	);
	platform->setUUID(-4);

	// Client side non-moving entities
	entityController->insertEntity(*ground);
	entityController->insertEntity(*platform);
	entityController->insertEntity(*ball);

	// Get current window size
	int* w = new int(0);
	int* h = new int(0);
	SDL_GetWindowSizeInPixels(display->window, w, h);
	std::cout << "WindowSize " << *w << ", " << *h << "\n";

	// Sets global scaling to 80 by default (1280x720), 
	// but sets it to not be on proportional scaling by default
	globalScaling = Utils::Vector2D((float) *w / (float) DEFAULT_SCALING_WIDTH, (float) *h / (float) DEFAULT_SCALING_HEIGHT);
	proportionalScalingActive = false;

	std::cout << "globalScaling: " << globalScaling.x << "x" << globalScaling.y << "\n";

	// TODO Multithreading
	// Basic, primitive game loop
	// TODO: Add ability to reload everything via terminal at some point
	while (true) {
		if (playerController) {
			// TODO: Send client information update to the server
			// Update request and subscriber
			Client::run(&serverToClientSubscriber, &clientToServerRequest, &clientToServerPublisher, player, entityController);

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

			// check player actionInput (TESTING)
			playerController->actionInput();

			// Update the physics of all entities
			entityController->updateEntities();

			// TODO: Receive game state updates from server

			// Respawn Player
			if (player->getPosition()->y > 1000) {
				player->setPosition(250.0f, 400.0f);
			}

			// Display all entities
			std::map<int, Entities::Entity>::iterator iterEnt;

			// Loop through entities and display them all
			for (iterEnt = entityController->getEntities()->begin(); iterEnt != entityController->getEntities()->end(); ++iterEnt) {
				Render::displayEntity((Entities::Entity) iterEnt->second);
			}

			/*Render::displayEntity((Entities::Entity)*player);
			Render::displayEntity(*ground);
			Render::displayEntity(*platform);
			Render::displayEntity(*ball);*/
			//Render::displayEntity((Entities::Entity)*movingBox);

			// Renders the scene gven the parameters identified in prepareScene()
			Render::presentScene();

			capFrameRate(&then, &remainder);
		}
	}

	delete w;
	delete h;

	//// Make sure both threads are complete before stopping main thread
	//first.join();
	//second.join();

	// Success condition
	return 0;
}

