#include <zmq.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <unordered_map>
#include "server.h"
#include "structs.h"

namespace Server {
    // Game state: A map of client IDs to player positions
    std::unordered_map<std::string, PlayerInfo> gameState;

    /**
    * Updates the Player's position on the server.
    * @param clientID the client's ID for reference
    * @param positionData the client's positin data formatted into a string
    */
    void updatePlayerPosition(const std::string& clientID, const std::string& positionData) {
        std::stringstream ss(positionData);
        float x, y, vx, vy;

        ss >> x >> y >> vx >> vy;

        // Update or insert player info in the game state
        gameState[clientID] = { x, y, vx, vy };
    }

    /**
    * Takes the game state from the server and serializes it into a string for broadcasting.
    * @return String of serialized game data
    */
    std::string serializeGameState() {
        std::stringstream ss;
        for (const auto& [clientID, playerInfo] : gameState) {
            ss << clientID << " " << playerInfo.x << " " << playerInfo.y << " "
                << playerInfo.vx << " " << playerInfo.vy << " ";
        }
        return ss.str();
    }

    /*
    * TODO Server-side code with main loop. (I am not sure how to run this and test it as of now)
    * Broadcasts updated positions of all players to all clients using PUB-SUB pattern
    * Created from tutorials within: https://zguide.zeromq.org/docs/chapter1/, https://zguide.zeromq.org/docs/chapter4/, and https://zguide.zeromq.org/docs/chapter5/
    */ 
    int run() {
        std::cout << "Running server.\n";
        // Set up ZMQ context and sockets
        zmq::context_t context(2);

        // REP socket for receiving player updates from clients
        zmq::socket_t rep_socket(context, zmq::socket_type::rep); // REQ-REP
        rep_socket.bind("tcp://*:5555");  // Server binds to port 5555 for requests

        // PUB socket for broadcasting game state to all clients
        zmq::socket_t pub_socket(context, zmq::socket_type::pub); // PUB-SUB
        pub_socket.bind("tcp://*:5556");  // Server publishes updates to port 5556

        std::cout << "Server is running..." << std::endl;

        while (true) {
            // Receive player updates from clients
            zmq::message_t request;
            rep_socket.recv(request, zmq::recv_flags::none);

            // Parse the request (client ID and position data)
            std::string request_data(static_cast<char*>(request.data()), request.size());
            std::stringstream ss(request_data);
            std::string clientID, positionData;
            ss >> clientID;
            std::getline(ss, positionData);

            // Update game state based on the received data
            updatePlayerPosition(clientID, positionData);

            // Send an acknowledgment back to the client
            zmq::message_t reply(5);
            memcpy(reply.data(), "ACK", 5);
            rep_socket.send(reply, zmq::send_flags::none);

            // Serialize the current game state and broadcast it to all clients
            std::string game_state = serializeGameState();
            zmq::message_t game_state_msg(game_state.c_str(), game_state.size());
            pub_socket.send(game_state_msg, zmq::send_flags::none);

            // Simulate a game tick
            std::this_thread::sleep_for(std::chrono::milliseconds(16));  // Approx 60fps
        }

        return 0;
    }
}