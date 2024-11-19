#pragma once

#ifndef PEERTOPEER_H
#define PEERTOPEER_H

#include <zmq.hpp>
#include <iostream>
#include <string>
#include "structs.h"

#include "playerGO.h"
#include "gameObjectManager.h"
#include "configIO.h"

#include <thread>
#include <queue>


namespace PeerToPeer {
	/**
	* Sets up client parameters and interactions
	* @param subscriber Subscriber to setup
	* @param request Request to setup
	*/
	int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber,
		GameObjectManager*& gameObjectManager, ConfigSettings config, std::vector<GameObject*> spawnPoints, std::vector<std::thread> *threads, ClientIDSet* clientIDSet);

	/**
	* Run the networking communication setup
	* @param subscriber Subscriber to use
	*/
	int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, PlayerGO*& player,
		GameObjectManager*& gameObjectManager, std::vector<std::thread>* threads, ClientIDSet* clientIDSet);
}

#endif