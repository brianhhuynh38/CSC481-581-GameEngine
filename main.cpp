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

/// The resolution width the window is set to
int resolutionWidth;
/// The resolution Height the window is set to
int resolutionHeight;

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
		resolutionWidth, resolutionHeight, windowFlags);
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

	std::cout << *then;
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
	zmq::context_t context(1);
	zmq::socket_t client_socket(context, zmq::socket_type::req);  // Request socket for client-to-server communication (REQ-REP)
	zmq::socket_t subscriber_socket(context, zmq::socket_type::sub);  // Subscribe socket for receiving updates from server (PUB-SUB)

	// TODO: Connect client to server (server's address will need to be specified)
	client_socket.connect("tcp://localhost:5555");  // Port 5555 for requests
	subscriber_socket.connect("tcp://localhost:5556");  // Server's PUB socket on Port 5556
	subscriber_socket.set(zmq::sockopt::subscribe, "");  // Subscribe to all messages

	// Loads in config file to read and get configured gravity
	loadConfigFile();
	
	// Initialize SDL components
	initSDL();

	// Runs memoryCleanUp() if the application exits
	std::atexit(memoryCleanUp);

	// Initiate current stage
	//initStage();

	then = timeline.getTime();
	remainder = 0;

	// Create a player Entity (Temp: Make more malleable in the future)
	// TODO: Base starting position off window size percentage
	player = new Entities::Player(
		1.0, 1.0,
		250.0, 250.0,
		15.0, 25.0,
		50.0,
		"./Assets/Textures/DefaultPlayerTexture1.png",
		false,
		true,
		0.0f, -150.0f,
		6.0
	);
	// Create ball object (Temp)
	ball = new Entities::Entity(
		1.0, 1.0,
		550.0, 250.0,
		20.0, 20.0,
		10.0,
		"./Assets/Textures/BallTexture.png",
		true,
		false
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
		false,
		true,
		false,
		10,
		500,
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
	entityController->addEntity((Entities::Entity) *player);
	entityController->addEntity(*ground);
	entityController->addEntity(*platform);
	entityController->addEntity(*ball);
	entityController->addEntity((Entities::Entity) *movingBox);
	entityController->addMovingEntity(*movingBox);
	
	// TODO: Networking: Initial game state request to the server. Currently errors, FIX (likely in other server or client .cpp files)
	/*zmq::message_t init_request(5);
	memcpy( init_request.data(), "Hello", 5);
	client_socket.send(init_request, zmq::send_flags::none);*/

	// Get current window size
	int* w = new int(0);
	int* h = new int(0);
	SDL_GetWindowSizeInPixels(display->window, w, h);
	std::cout << "WindowSize " << *w << ", " << *h;

	// Sets global scaling to 80 by default (1280x720), 
	// but sets it to not be on proportional scaling by default
	globalScaling = Utils::Vector2D((float) *w / (float) DEFAULT_SCALING_WIDTH, (float) *h / (float) DEFAULT_SCALING_HEIGHT);
	proportionalScalingActive = false;

	std::cout << "globalScaling: " << globalScaling.x << "x" << globalScaling.y;

	// Network multithreading

	// Mutex to handle locking, condition variable to handle notifications between threads
    std::mutex m;
    std::condition_variable cv;

    // Create thread objects
    NetworkThread serverThread(0, NULL, &m, &cv);
    NetworkThread clientThread(1, &serverThread, &m, &cv);

    std::cout << "Starting network threads \n";

    
    

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

		// TODO: Send player position update to the server
		std::string movement_data = Client::serializePlayerMovement(player);  // Client function to serialize player movement
		zmq::message_t movement_request(movement_data.c_str(), movement_data.size());
		client_socket.send(movement_request, zmq::send_flags::none);

		// TODO: Receive game state updates from server
		zmq::message_t game_state_update;
		if (subscriber_socket.recv(game_state_update, zmq::recv_flags::dontwait)) {
			std::string game_state(static_cast<char*>(game_state_update.data()), game_state_update.size());
			Client::updateGameState(game_state);  // Client function to update player positions from the server
		}

		// Update and run threads (temp)
		std::thread first(NetworkThread::run, &serverThread);
		std::thread second(NetworkThread::run, &clientThread);

		// TEST PRINT for player info (DELETE LATER)
		//std::cout << "Player P(" << player->getPosition()->x << ", " << player->getPosition()->y << ") | V(" << player->getVelocity()->x << ", " << player->getVelocity()->y << ") | A(" << player->getAcceleration()->x << ", " << player->getAcceleration()->y << ") | Grounded(" << player->getIsGrounded() << ")\n";

		// Display player and floor texture at their locations
		Render::displayEntity((Entities::Entity) *player);
		Render::displayEntity(*ground);
		Render::displayEntity(*platform);
		Render::displayEntity(*ball);
		Render::displayEntity((Entities::Entity) *movingBox);

		// Renders the scene gven the parameters identified in prepareScene()
		Render::presentScene();

		capFrameRate(&then, &remainder);
	}

	delete w;
	delete h;

	// Make sure both threads are complete before stopping main thread
	first.join();
	second.join();

	// Success condition
	return 0;
}

