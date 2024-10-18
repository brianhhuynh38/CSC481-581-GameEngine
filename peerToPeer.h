#pragma once
#include <zmq.hpp>
#include <iostream>
#include <string>
#include "entity.h"
#include "player.h"
#include "structs.h"
#include "playerController.h"

#ifndef PEERTOPEER_H
#define PEERTOPEER_H

namespace PeerToPeer {
	/**
	* Sets up client parameters and interactions
	* @param subscriber Subscriber to setup
	* @param request Request to setup
	*/
	int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, Entities::Player*& player, EntityController*& entityController, Controllers::PlayerController*& playerController, ConfigSettings config);

	/**
	* Run the networking communication setup
	* @param subscriber Subscriber to use
	*/
	int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, Entities::Player*& player, EntityController*& entityController);
}

#endif