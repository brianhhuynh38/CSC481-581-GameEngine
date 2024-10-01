#pragma once
#include <zmq.hpp>
#include <iostream>
#include <string>
#include "entity.h"
#include "player.h"
#include "structs.h"
#include "playerController.h"

#ifndef CLIENT_H
#define CLIENT_H

namespace Client {
	/**
	* Sets up client parameters and interactions
	* @param subscriber Subscriber to setup
	* @param request Request to setup
	*/
	int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* publisher, Entities::Player*& player, EntityController*& entityController, Controllers::PlayerController*& playerController);

	/**
	* Run the networking communication setup
	* @param subscriber Subscriber to use
	*/
	int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* publisher, Entities::Player player, EntityController*& entityController);
}

#endif