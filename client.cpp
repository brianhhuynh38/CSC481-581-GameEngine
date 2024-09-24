#include <zmq.hpp>
#include <iostream>
#include <string>
#include "entity.h"
#include "player.h"
#include <unordered_map>
#include "server.h"
#include "structs.h"

namespace Client {
    // Map to store players by their ID from the client
    std::unordered_map<std::string, PlayerInfo> players;

    /**
    * Puts the player's movement information into a string.
    * @param player Player object to get movement from.
    * @return Player movement serialized into a string
    */
    std::string serializePlayerMovement(Entities::Player* player) {
        // Convert player position and movement data to a string
        std::stringstream ss;
        ss << player->getPosition()->x << "," << player->getPosition()->y;
        return ss.str();
    }

    /**
    * This function updates the client game state by parsing the game state string from the server.
    * It updates the position and velocity of each player on the client side.
    *
    * @param gameState The serialized game state string received from the server.
    */
    void updateGameState(const std::string& gameState) {
        std::stringstream ss(gameState);
        std::string clientID;
        float x, y, vx, vy;

        // Temporary map to hold updated players, this will help to handle players who may have disconnected
        std::unordered_map<std::string, PlayerInfo> updatedPlayers;

        // Parse the game state string received from the server
        while (ss >> clientID >> x >> y >> vx >> vy) {
            // If the player is already in the local players map, update their position and velocity
            if (players.find(clientID) != players.end()) {
                players[clientID].x = x;
                players[clientID].y = y;
                players[clientID].vx = vx;
                players[clientID].vy = vy;
            }
            else {
                // If the player is not yet tracked locally, add them
                players[clientID] = { x, y, vx, vy };
            }

            // Store the updated player in the temporary map
            updatedPlayers[clientID] = players[clientID];
        }

        // Replace the current player list with the updated one
        players = std::move(updatedPlayers);

        // Print game state (for debugging)
        std::cout << "Updated game state from server:\n";
        for (const auto& [id, playerInfo] : players) {
            std::cout << "Player " << id << ": Position(" << playerInfo.x << ", " << playerInfo.y
                << "), Velocity(" << playerInfo.vx << ", " << playerInfo.vy << ")\n";
        }
    }

    // Client main loop is handled within main.cpp. TODO fix functionality and un-comment out
    // Sends movement updates to the server using REQ-REP pattern.
}