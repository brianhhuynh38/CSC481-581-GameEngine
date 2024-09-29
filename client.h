#pragma once
#include <zmq.hpp>
#include <iostream>
#include <string>
#include "entity.h"
#include "player.h"
#include "structs.h"
#ifndef CLIENT_H
#define CLIENT_H

namespace Client {
	/**
	* Puts the player's movement information into a string.
	* @param player Player object to get movement from.
	* @return Player movement serialized into a string
	*/
    std::string serializePlayerMovement(Entities::Player* player);

	/**
	* This function updates the client game state by parsing the game state string from the server.
    * It updates the position and velocity of each player on the client side.
	* @param gameState The serialized game state string received from the server.
	*/
    void updateGameState(const std::string& game_state);

	/**
	* Sets up client parameters and interactions
	* @param subscriber Subscriber to setup
	* @param request Request to setup
	*/
	int startup(zmq::socket_t* subscriber, zmq::socket_t* request);

	/**
	* Run the networking communication setup
	* @param subscriber Subscriber to use
	*/
	int run(zmq::socket_t* subscriber, zmq::socket_t* request);
}

#endif