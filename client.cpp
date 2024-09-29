#include <iostream>
#include <string>
#include "entity.h"
#include "player.h"
#include <unordered_map>
#include "structs.h"
#include <chrono>
#include <thread>

#include <zmq.hpp>
#include <zmq_addon.hpp>

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
        ss << player->getVelocity()->x << "," << player->getVelocity()->y;
        ss << player->getAcceleration()->x << "," << player->getAcceleration()->y;
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

    /**
    * Sets up client parameters and interactions
    * @param subscriber Subscriber to setup
    * @param request Request to setup
    */
    int startup(zmq::socket_t *subscriber, zmq::socket_t *request) {
        subscriber->connect("tcp://localhost:5555");
        request->connect("tcp://localhost:5556");

        // Send message to server and wait to receive client identifier
        request->send(zmq::str_buffer("Requesting client identifier"));

        // Sleep for 2 seconds to guarantee connection
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Receive response from server 
        zmq::message_t reply;
        request->recv(reply, zmq::recv_flags::none);
        std::cout << "Received client identifier: " << reply.to_string() << "," << reply.to_string().length() << "\n";
        //assert((reply.to_string() == "Client_1"));
        // Convert the client identifier to an integer
        // int clientNumber = stoi;
        //sscanf_s(reply.to_string().c_str(), "%d", &clientNumber);

        // Set the client identifier
        //subscriber->set(zmq::sockopt::subscribe, "Client");

        // Set the client identifier (SECTION 5)
        subscriber->set(zmq::sockopt::subscribe, reply.to_string());

        return 0;
    }

    /**
    * Run the networking communication setup
    * @param subscriber Subscriber to use
    */
    int run(zmq::socket_t* subscriber, zmq::socket_t* request) {
        // Receive messages from the server as a subscriber
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result =
            zmq::recv_multipart(*(subscriber), std::back_inserter(recv_msgs), zmq::recv_flags::dontwait);
        //assert(result && "recv failed");
        //assert(*result == 2);

        if (recv_msgs.size() > 0) {
            std::cout << recv_msgs[0].to_string() << recv_msgs[1].to_string() << std::endl;
        }

        return 0;
    }
}