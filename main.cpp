#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <zmq.hpp>
#include <string>
#include <mutex>
#include <set>

#include "definitions.h"
#include "draw.h"
#include "input.h"
#include "global.h"
#include "structs.h"
#include "configIO.h"
#include "physicsCalculator.h"

#include "spawnEvent.h"

#include "eventManager.h"
#include "gameObjectManager.h"

#include "client.h"
#include "peerToPeer.h"

#include "recorder.h"

#include "GameObject.h"
#include "staticObject.h"
#include "transform.h"
#include "playerGO.h"
#include "deathZone.h"
#include "boundaryZone.h"

// Global variables

// The Display struct used to initialize renderer and window
Display *display;
// Global scale factor
Utils::Vector2D globalScaling;
// Determines if proportional scaling is active
bool proportionalScalingActive;
// Manager in charge of all queueing and execution of events
EventManager* eventManager;
// Recorder that records all events being recorded
Recorder* recorder;
/** Whether or not this client is recording their events */
bool isRecording = false;
/** Whether or not a playback is in process */
bool startPlayback = false;

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
	PhysCalc::setGravity(settings.gravity);

	// Initialize SDL components
	initSDL(settings);

	// Runs memoryCleanUp() if the application exits
	std::atexit(memoryCleanUp);

	then = timeline->getTime();
	remainder = 0;

	
	// Global mutex and condition variable to be used across threads
	std::mutex renderMtx;
	std::condition_variable renderCV;

	

	// Create gameObjectManager
	GameObjectManager* gameObjectManager = new GameObjectManager(timeline);

	// Create Recorder
	recorder = new Recorder(gameObjectManager);

	// Create EventManager
	eventManager = new EventManager(recorder);

	InputHandler* inputHandler = new InputHandler();
	Input* input = new Input(inputHandler);

	// Test SpawnPoints
	std::vector<GameObject*> spawnPoints{
		new GameObject(
			1.0f, 1.0f,
			350.0f, 400.0f,
			32.0f, 32.0f
		),
		new GameObject(
			1.0f, 1.0f,
			450.0f, 400.0f,
			32.0f, 32.0f
		),
		new GameObject(
			1.0f, 1.0f,
			550.0f, 400.0f,
			32.0f, 32.0f
		)
	};

	Utils::Vector2D *cameraPosition = new Utils::Vector2D();

	// Test StaticObject
	StaticObject* platformObject = new StaticObject(1.0, 1.0, 350.0, 450.0, 1000.0, 64.0, 10.0, "./Assets/Textures/devLongTexture2.png", true);

	// Test other StaticObject
	StaticObject* ball = new StaticObject(1.0, 1.0, 550.0, 200.0, 20.0, 20.0, 10.0, "./Assets/Textures/BallTexture.png", true);

	// Test DeathZone
	DeathZone* deathZone = new DeathZone(1.0, 1.0, 0.0, 650.0, 2000.0, 64.0, 10.0, "./Assets/Textures/devLongTexture4.png", true);

	// Test Boundary
	BoundaryZone* boundaryZone = new BoundaryZone(
		1.0f, 1.0f,
		600.0f, 200.0f,
		1.0f, 1000.0f,
		cameraPosition,
		50.0f,
		"./Assets/Textures/wall.png",
		true,
		Utils::Vector2D(600.0f, 0.0f),
		Utils::Vector2D(0.0f, 0.0f)
	);

	gameObjectManager->insert(boundaryZone);
	gameObjectManager->insert(deathZone);
	gameObjectManager->insert(platformObject);
	gameObjectManager->insert(ball);

	// Get current window size
	int* w = new int(0);
	int* h = new int(0);
	SDL_GetWindowSizeInPixels(display->window, w, h);
	std::cout << "WindowSize " << *w << "x" << *h << "\n";

	// Sets global scaling to 80 by default (1280x720), 
	// Sets it to be on proportional scaling by default
	globalScaling = Utils::Vector2D((float) *w / (float) DEFAULT_SCALING_WIDTH, (float) *h / (float) DEFAULT_SCALING_HEIGHT);
	proportionalScalingActive = true;

	// Create a vector of client threads so that they can be joined later on
	std::vector<std::thread> clientThreads = std::vector<std::thread>();

	std::cout << "globalScaling: " << globalScaling.x << "x" << globalScaling.y << "\n";

	int currentRenderingID = 0;

	//std::mutex playerMutex;

    // Client ID queue that tracks any new clients that join the game
    ClientIDSet* clientIDSet = new ClientIDSet();
    clientIDSet->idSet = std::set<int>();
    clientIDSet->instantiatedIDs = std::set<int>();

	// Update request and subscriber. Put on a new thread
	if (settings.networkType == 2) {
		PeerToPeer::startup(
			&serverToClientSubscriber,
			&clientToServerRequest,
			&peerToPeerPublisher,
			&peerToPeerSubscriber,
			gameObjectManager,
			settings,
			spawnPoints,
			&clientThreads,
			clientIDSet
		);
	}
	else {
		// This is the startup function for the Client-Server implementation, which is now [DEPRECATED]

		//Client::startup(
		//	&serverToClientSubscriber,
		//	&clientToServerRequest,
		//	&clientToServerPublisher,
		//	playerObject,
		//	gameObjectManager,
		//	settings,
		//	spawnPoints
		//);
	}

	// Get player object created during the network startup process
	PlayerGO* playerObject = gameObjectManager->tryGetPlayer();

	// Set the input handler in main
	{
		//std::lock_guard<std::mutex> lock(playerMutex);
		playerObject->getComponent<Components::PlayerInputPlatformer>()->setInputHandler(inputHandler);
	}

	std::thread gameObjectThread([&]() {
		while (true) {
			
			//std::unique_lock<std::mutex> lock(recordingMutex); // Lock the mutex
			//renderCV.wait(lock, [&] { return currentRenderingID == false; });
			//std::cout << "GameObject thread loop.\n";
		
			// Safely update the player object with the new deltaTime
			timeline->updateTime(); // Update the timeline for deltaTime

			// Safely update all GameObjects and Events
			if (!startPlayback) {
				// Update GameObject components
				gameObjectManager->update();
			}
			// Dispatch events
			eventManager->dispatchEvents(timeline->getTime());

			std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Sleep to control thread timing
		}
	});

	std::thread inputThread([&]() {
		while (true) {

			if (!startPlayback) {
				// Handles player input, including exit
				input->takeInput();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Sleep to control thread timing
		}
	});

	while (true) {
		// Update request and subscriber
		if (!startPlayback) {
			// Safely run the networking code
			if (settings.networkType == 2) {
				PeerToPeer::run(
					&serverToClientSubscriber,
					&clientToServerRequest,
					&peerToPeerPublisher,
					&peerToPeerSubscriber,
					playerObject,
					gameObjectManager,
					&clientThreads,
					clientIDSet
				);
			}
			else {
				Client::run(&serverToClientSubscriber, &clientToServerRequest, &clientToServerPublisher, playerObject, gameObjectManager);
			}
		}

		// Prepares scene for rendering
		Render::prepareScene();


		//std::cout << "Player Position: " << playerObject->getComponent<Components::Transform>()->getPosition()->toString();

		// Updates the keyboard inputs
		SDL_PumpEvents();

		// Display all GameObjects
		std::map<int, GameObject*>::iterator iterGO;
		// Get object map
		std::map<int, GameObject*> objectMap = *gameObjectManager->getObjectMap();

		//std::cout << "\n\n\nStart Rendering...\n\n\n";

		// Loop through objects and display them all
		for (iterGO = objectMap.begin(); iterGO != objectMap.end(); ++iterGO) {
			GameObject* go = iterGO->second;
			{
				std::lock_guard<std::mutex> lock(go->mutex);
				Render::displayGameObject(*iterGO->second, *cameraPosition);
			}
		}

		//// Death plane because the player teleports sometimes
		//if (playerObject->getComponent<Components::Transform>()->getPosition()->y > 1000) {
		//	std::vector<GameObject*> goVec = std::vector<GameObject*>();
		//	goVec.push_back(playerObject);
		//	Events::SpawnEvent* se = new Events::SpawnEvent(goVec, playerObject->getCurrentTimeStamp(), 1);
		//	eventManager->raiseEvent(se);
		//}

		// Try play back recording if applicable
		recorder->tryDispatchRecording(timeline->getTime());

		// Renders the scene gven the parameters identified in prepareScene()
		Render::presentScene();

		//SDL_Delay(16);
		capFrameRate(timeline, &then, &remainder);
	}

	gameObjectThread.join();
	inputThread.join();
	//networkThread.join();

	delete w;
	delete h;

	// Success condition
	return 0;
}

