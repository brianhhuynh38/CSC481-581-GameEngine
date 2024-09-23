#pragma once
#include <zmq.hpp>
#include <iostream>
#include <string>
#include "entity.h"
#include "player.h"
#include "structs.h"
#ifndef SERVER_H
#define SERVER_H

namespace Server {
	/**
	* Updates the Player's position on the server.
	* @param clientID the client's ID for reference
	* @param positionData the client's positin data formatted into a string
	*/
    void updatePlayerPosition(const std::string& clientID, const std::string& positionData);

	/**
	* Takes the game state from the server and serializes it into a string for broadcasting.
	* @return String of serialized game data
	*/
    std::string serializeGameState();
}

#endif