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
#include "entityController.h"
#include "playerController.h"
#include "configIO.h"

namespace PeerToPeer {
    // Map to store players by their ID from the client
    std::unordered_map<std::string, PlayerInfo> players;

    /**
    * Sets up client parameters and interactions
    * @param subscriber Subscriber to setup
    * @param request Request to setup
    */
    int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher,
    zmq::socket_t* p2psubscriber, Entities::Player*& player, EntityController*& entityController,
    Controllers::PlayerController*& playerController, ConfigSettings config) {
        // Set socket options
        int conflate = 1;
        zmq_setsockopt(subscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        subscriber->connect("tcp://localhost:5555");
        request->connect("tcp://localhost:5556");
        
        zmq::message_t msg(std::to_string(config.networkType));
        // Send message to server and wait to receive client identifier
        request->send(msg, zmq::send_flags::none);

        // Sleep for 2 seconds to guarantee connection
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Receive response from server 
        zmq::message_t reply;
        request->recv(reply);
        //std::cout << "Received client identifier: " << reply.to_string() << "\n";

        // Should continue from after uuid
        std::string playerString = reply.to_string();
        //playerString.erase(0, playerString.find("\n") + 1);

        //std::cout << "Printing fresh playerstring off server: \n" << playerString << "\n";

        player = Entities::Player::fromString(playerString);

        int portNum = 5558 + player->getUUID();

        std::stringstream ss;
        ss << "tcp://localhost:" << portNum;

        // Set to only receive last message (theoretically)
        zmq_setsockopt(p2ppublisher, ZMQ_CONFLATE, &conflate, sizeof(conflate));
        zmq_setsockopt(p2psubscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        p2ppublisher->bind(ss.str());
        entityController->setPlayerID(player->getUUID());

        // Set the client identifier (SECTION 5)
        subscriber->set(zmq::sockopt::subscribe, "");
        p2psubscriber->set(zmq::sockopt::subscribe, "");

        return 0;
    }

    /**
    * Run the networking communication setup
    * @param subscriber Subscriber to use
    */
    int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, Entities::Player*& player, EntityController*& entityController) {
        // Receive messages from the server as a subscriber for MovingEntities only
        zmq::message_t serverInfo;
        zmq_connect(subscriber, "tcp://localhost:5555");
        subscriber->recv(serverInfo, zmq::recv_flags::dontwait);
        zmq_disconnect(subscriber, "tcp://localhost:5555");

        if (!serverInfo.empty()) {
            std::stringstream ss;
            ss.clear();
            ss.str(serverInfo.to_string());

            // Read the first line to extract the clock value
            std::string firstLine;
            std::getline(ss, firstLine);

            // Now, extract the clock from the first line
            std::stringstream clockStream(firstLine);
            int64_t clock;
            if (!(clockStream >> clock)) {
                std::cerr << "Failed to parse clock value." << std::endl;
                // return -1;
            }

            // Calculate the delay (SUICIDE-SNAIL SOLUTION)
            const auto delay = std::clock() - clock;
            if (delay > MAX_ALLOWED_DELAY) {
                std::cerr << "E: subscriber cannot keep up, aborting. Delay=" << delay << std::endl;
                // return -1;
            }

            entityController->updateEntitiesByString(serverInfo.to_string(), 2);
        }

        // PEER TO PEER
        zmq::message_t playerInfo("Client\n" + player->toString());

        // Iterate through opposing players
		std::map<int, Entities::Entity>::iterator iter;
        std::map<int, Entities::Entity> playerMap = *entityController->getOpposingPlayers();

		// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
		for (iter = playerMap.begin(); iter != playerMap.end(); ++iter) {

			int entityId = iter->first; // Get the ID of the current entity
            std::cout << "Hello, playerMap is populated\n";

            // Calculate port and connect with both
            int portNum = 5558 + entityId;
            std::stringstream ss;
            ss << "tcp://localhost:" << portNum;
            p2ppublisher->connect(ss.str());
            p2psubscriber->connect(ss.str());

            //send messages
            p2ppublisher->send(playerInfo, zmq::send_flags::dontwait);

            // Receive client info
            zmq::message_t clientInfo;
            p2psubscriber->recv(clientInfo, zmq::recv_flags::dontwait);

            if (!clientInfo.empty()) {
                std::cout << "Print peer's player info: " << clientInfo.to_string() << "\n";
                // Create player from string and update their information
                Entities::Player updatedPlayer = *Entities::Player::fromString(clientInfo.to_string());
                // Update the player using the string 
                entityController->insertOpposingPlayer(updatedPlayer);
                entityController->insertEntity(updatedPlayer);
                std::cout << "Player toString: " << updatedPlayer.toString() << "\n";
            }
        }
        return 0;
    }

    int startThread(zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, Entities::Player*& player) {

        // Get client port number
        int portNum = 5558 + player->getUUID();
        // Assemble network address
        std::stringstream ss;
        ss << "tcp://localhost:" << portNum;

        // Set to only receive last message
        int conflate = 1;
        zmq_setsockopt(p2ppublisher, ZMQ_CONFLATE, &conflate, sizeof(conflate));
        zmq_setsockopt(p2psubscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        // Bind and set socket options and addresses
        p2ppublisher->bind(ss.str());
        p2psubscriber->set(zmq::sockopt::subscribe, "");
        
        // Connect each publisher and subscriber to the address
        p2ppublisher->connect(ss.str());
        p2psubscriber->connect(ss.str());

        return 0;
    }

    int runThread(zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, Entities::Player*& player) {
        zmq::message_t playerInfo("Client\n" + player->toString());

        //send messages
        p2ppublisher->send(playerInfo, zmq::send_flags::dontwait);

        // Receive client info
        zmq::message_t clientInfo;
        p2psubscriber->recv(clientInfo, zmq::recv_flags::dontwait);

        if (!clientInfo.empty()) {
            std::cout << "Print peer's player info: " << clientInfo.to_string() << "\n";
            // Create player from string and update their information
            Entities::Player updatedPlayer = *Entities::Player::fromString(clientInfo.to_string());

            // TODO: Add update function for each player here

            std::cout << "Player toString: " << updatedPlayer.toString() << "\n";
        }
    }
}