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
#include "configIO.h"

#include "entity.h"
#include "player.h"
#include "movingEntity.h"

#include "playerController.h"
#include "entityController.h"

#include "client.h"
#include "peerToPeer.h"

#include "GameObject.h"
#include "staticObject.h"
#include "transform.h"

// Global variables
/// The Display struct used to initialize renderer and window
Display *display;
/// Controller for all entities and their physics
/// Global scale factor
Utils::Vector2D globalScaling;
/// Determines if proportional scaling is active
bool proportionalScalingActive;

/**
 * Frees any allocated memory on application exit
 */
void memoryCleanUp() {
	delete display;
	// Free memory for player
	//player->destroy();
	//delete player;
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
static void capFrameRate(Timeline *timeline, long* then, float* remainder) {
	long wait, frameTime;

	const int targetFrameTime = 16667;

	frameTime = timeline->getTime() - *then;

	wait = targetFrameTime - frameTime + static_cast<int>(*remainder);

	if (wait < 1000) {
		wait = 1000;
	}

	SDL_Delay(wait/1000);

	*remainder += (wait % 1000) / 1000.0f;

	*then = timeline->getTime();
}


/**
 * The main function where all components are initialized and the gameplay update loop is maintained 
 */
int main(int argc, char* argv[]) {
	long then;
	float remainder;
	
	// Allocate memory to SDL renderer components and instantiate Display struct
	display = new Display;

	/// The timeline used to keep track of time intervals
	Timeline *timeline = new Timeline();
	// Create physics
	Physics *physics = new Physics();
	
	// TODO: Initialize networking
	// initialize the zmq context with a single IO thread
	zmq::context_t context{ 6 };
	// construct a SUB (subscribe) socket to receive entity movements and checks from the server
	zmq::socket_t serverToClientSubscriber{ context, zmq::socket_type::sub };
	// construct a REQ (request) socket to receive client identification information from the server
	zmq::socket_t clientToServerRequest{ context, zmq::socket_type::req };
	// construct a PUB (publish) socket to send player information to the server
	zmq::socket_t clientToServerPublisher{ context, zmq::socket_type::pub };

	// PEER TO PEER

	// construct a PUB (publish) socket to send player information to the server
	zmq::socket_t peerToPeerPublisher{ context, zmq::socket_type::pub };
	// construct a SUB (subscribe) socket to receive entity movements and checks from the server
	zmq::socket_t peerToPeerSubscriber{ context, zmq::socket_type::sub };
	// Listens for peer to peer client connects
	zmq::socket_t clientListener{ context, zmq::socket_type::pull };

	// Struct that contains all settings defined in configuration file
	ConfigSettings settings = ConfigSettings();
	
	// Loads in config file to read and get configured gravity
	loadConfigFile(&settings);

	// Set physics configured gravity
	physics->setGravity(settings.gravity);
	// Initialize SDL components
	initSDL(settings);

	// Runs memoryCleanUp() if the application exits
	std::atexit(memoryCleanUp);

	then = timeline->getTime();
	remainder = 0;

	// Create the entityController
	EntityController* entityController = new EntityController(physics);
	
	// The entity that the player is able to control
	Entities::Player* player;
	
	// The default player controller
	Controllers::PlayerController* playerController;

	// Create a vector of client threads so that they can be joined later on
	std::vector<std::thread> clientThreads = std::vector<std::thread>();

	// Update request and subscriber. Put on a new thread
	if (settings.networkType == 2) {
		PeerToPeer::startup(&serverToClientSubscriber, &clientToServerRequest, &peerToPeerPublisher, &peerToPeerSubscriber, player, entityController, playerController, settings);
	}
	else {
		Client::startup(&serverToClientSubscriber, &clientToServerRequest, &clientToServerPublisher, player, entityController, playerController);
	}
	

	playerController = new Controllers::PlayerController(player, entityController, timeline);

	InputHandler* inputHandler = new InputHandler();

	Input* input = new Input(inputHandler);

	GameObject* gameObject = new GameObject();
	Components::Transform *transform = gameObject->getComponent<Components::Transform>();
	transform->setPosition(5.0, 5.0);

	StaticObject* ball = new StaticObject(1.0, 1.0, 550.0, 250.0, 20.0, 20.0, nullptr, 10.0, "./Assets/Textures/BallTexture.png",true,false, physics);
	//ball->setUUID(-2);

	/*Entities::Entity* ball = new Entities::Entity
	(
		1.0, 1.0,
		550.0, 250.0,
		20.0, 20.0,
		10.0,
		"./Assets/Textures/BallTexture.png",
		true,
		false
	);*/
	//ball->setUUID(-2);
	// Create ground object (Temp)
	Entities::Entity* ground = new Entities::Entity(
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
	Entities::Entity* platform = new Entities::Entity(
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
	//entityController->insertEntity(*ball);

	// Get current window size
	int* w = new int(0);
	int* h = new int(0);
	SDL_GetWindowSizeInPixels(display->window, w, h);
	std::cout << "WindowSize " << *w << "x" << *h << "\n";

	// Sets global scaling to 80 by default (1280x720), 
	// Sets it to be on proportional scaling by default
	globalScaling = Utils::Vector2D((float) *w / (float) DEFAULT_SCALING_WIDTH, (float) *h / (float) DEFAULT_SCALING_HEIGHT);
	proportionalScalingActive = true;

	std::cout << "globalScaling: " << globalScaling.x << "x" << globalScaling.y << "\n";

	std::mutex *m;
	std::condition_variable* cv;

	std::thread entityThread([&]() {
		while (true) {
			// EntityController thread

			// Update the physics of all entities
			entityController->updateEntities(timeline);

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	});

	std::thread playerThread([&]() {
		while (true) {
			// PlayerController thread

			// Updates to get a new deltaTime
			timeline->updateTime();

			// Handle movement inputs and actions
			playerController->movementInput(inputHandler);

			playerController->actionInput(inputHandler);

			// Handle physics
			playerController->updatePlayerPhysics(physics);

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	});

	// TODO Multithreading
	// Basic, primitive game loop
	// TODO: Add ability to reload everything via terminal at some point
	while (true) {
		// TODO: Send client information update to the server
		// Update request and subscriber
		if (settings.networkType == 2) {
			PeerToPeer::run(&serverToClientSubscriber, &clientToServerRequest, &peerToPeerPublisher, &peerToPeerSubscriber, player, entityController);
		}
		else {
			Client::run(&serverToClientSubscriber, &clientToServerRequest, &clientToServerPublisher, player, entityController);
		}

		// Prepares scene for rendering
		Render::prepareScene();

		// Updates the keyboard inputs
		SDL_PumpEvents();
		// Handles player input, including exit
		input->takeInput();

		//std::cout << "Acc: " << player->getAcceleration()->x << ", " << player->getAcceleration()->y << " | Vel:" << player->getVelocity()->x << ", " << player->getVelocity()->y << "\n";

		// Respawn Player
		if (player->getPosition()->y > 1000) {
			player->setPosition(250.0f, 400.0f);
		}

		// Display all entities
		std::map<int, Entities::Entity>::iterator iterEnt;
		// Get entity map
		std::map<int, Entities::Entity> entityMap = *entityController->getEntities();

		// Loop through entities and display them all
		for (iterEnt = entityMap.begin(); iterEnt != entityMap.end(); ++iterEnt) {
			Render::displayEntity((Entities::Entity) iterEnt->second);
		}
		
		//Render::displayGameObject((GameObject) *ball);

		Render::displayEntity((Entities::Entity) *player);

		// Renders the scene gven the parameters identified in prepareScene()
		Render::presentScene();

		//SDL_Delay(16);
		capFrameRate(timeline, &then, &remainder);
	}

	entityThread.join();
	playerThread.join();

	delete w;
	delete h;

	// Success condition
	return 0;
}

