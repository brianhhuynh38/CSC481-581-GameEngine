#pragma once

#ifndef CLIENT_H
#define CLIENT_H

#include <zmq.hpp>
#include <iostream>
#include <string>
#include "entity.h"
#include "player.h"
#include "structs.h"
#include "playerController.h"

#include "playerGO.h"
#include "gameObjectManager.h"
#include "configIO.h"

namespace Client {
	/**
	* Sets up client parameters and interactions
	* @param subscriber Subscriber to setup
	* @param request Request to setup
	*/
	int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* publisher,
		PlayerGO*& playerGO, GameObjectManager*& gameObjectManager, ConfigSettings config, std::vector<GameObject*> spawnPoints);

	/**
	* Run the networking communication setup
	* @param subscriber Subscriber to use
	*/
	int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* publisher, PlayerGO*& player,
		GameObjectManager*& gameObjectManager);
}

#endif