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
#include "movingEntity.h"
#include "configIO.h"
#include "playerController.h"
#include "entityController.h"

// Global variables
/// The Display struct used to initialize renderer and window
Display *display;
/// The entity that the player is able to control
Entities::Player *player;
/// Ball entity (TEST)
Entities::Entity *ball;
/// movingBox entity (TEST)
Entities::Entity *movingBox;
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
/// The physics system
Physics physics;
// The timeline used to keep track of time intervals
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
		1.0, 1.0,     // scale
		250.0, 450.0, // positon
		15.0, 25.0,   // width & height
		20.0,        // mass
		"./Assets/Textures/DefaultPlayerTexture1.png",
		false,
		true,
		0.0f, -250.0f, // jump vector
		32.0 // max speed
	);
	// Create ball object (Temp)
	ball = new Entities::Entity(
		1.0, 1.0,
		550.0, 250.0,
		20.0, 20.0,
		10.0,
		"./Assets/Textures/BallTexture.png",
		false,
		true
	);
	// Create box object that moves (Temp)
	movingBox = new Entities::MovingEntity
	(
		1.0, 1.0,
		550.0, 550.0,
		10.0,
		50.0f, 50.0f,
		"./Assets/Textures/devTexture0.png",
		false,
		true,
		true,
		false,
		10,
		5,
		800.0,
		800.0
	);
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

	// Create player controller for player (temp for testing)
	playerController = new Controllers::PlayerController(player);
	//Create entity controller
	entityController = new EntityController();
	entityController->addEntity(*player);
	entityController->addEntity(*ground);
	entityController->addEntity(*platform);
	entityController->addEntity(*ball);
	entityController->addEntity(*movingBox);
	
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

		// check player actionInput (TESTING)
		playerController->actionInput();

		// Update the physics of all entities
		entityController->updateEntities();

		//// TODO: Send player position update to the server
		//std::string movement_data = Client::serializePlayerMovement(player);  // Client function to serialize player movement
		//zmq::message_t movement_request(movement_data.c_str(), movement_data.size());
		//client_socket.send(movement_request, zmq::send_flags::none);

		//// TODO: Receive game state updates from server
		//zmq::message_t game_state_update;
		//if (subscriber_socket.recv(game_state_update, zmq::recv_flags::dontwait)) {
		//	std::string game_state(static_cast<char*>(game_state_update.data()), game_state_update.size());
		//	Client::updateGameState(game_state);  // Client function to update player positions from the server
		//}

		//// Update and run threads (temp)
		//std::thread serverThread(NetworkThread::run, &serverThread);
		//std::thread clientThread(NetworkThread::run, &clientThread);

		// TEST PRINT for player info (DELETE LATER)
		std::cout << "Player P(" << player->getPosition()->x << ", " << player->getPosition()->y << ") | V(" << player->getVelocity()->x << ", " << player->getVelocity()->y << ") | A(" << player->getAcceleration()->x << ", " << player->getAcceleration()->y << ") | Grounded(" << player->getIsGrounded() << ")\n";

		// Display player and floor texture at their locations
		Render::displayEntity(*player);
		Render::displayEntity(*ground);
		Render::displayEntity(*platform);
		Render::displayEntity(*ball);
		Render::displayEntity(*movingBox);

		// Renders the scene gven the parameters identified in prepareScene()
		Render::presentScene();

		

		// Slow down by 16 ms to maintain approximately 62 fps
		SDL_Delay(16);

		capFrameRate(&then, &remainder);
	}

	delete w;
	delete h;

	// Make sure both threads are complete before stopping main thread
	//serverThread.join();
	//clientThread.join();

	// Success condition
	return 0;
}

