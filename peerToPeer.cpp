#include "peerToPeer.h"

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
#include "gameObjectManager.h"

namespace PeerToPeer {
    // Map to store players by their ID from the client
    std::unordered_map<std::string, PlayerInfo> players;

    /**
    * Sets up client parameters and interactions
    * @param subscriber Subscriber to setup
    * @param request Request to setup
    */
    int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber,
        PlayerGO*& playerGO, GameObjectManager*& gameObjectManager, ConfigSettings config, std::vector<GameObject*> spawnPoints) {
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

        // Trim the identifier off the json
        std::string playerString = reply.to_string().substr(reply.to_string().find('\n') + 1);
        //playerString.erase(0, playerString.find("\n") + 1);

        std::cout << "Printing fresh playerstring off server: \n" << playerString << "\n\n\n";

        //player = Entities::Player::fromString(playerString);

        // Parse the json string received from the server into playerGO
        json j = json::parse(playerString);
        playerGO->from_json(j);

        // Sets a spawn point based off the size of the list of SpawnPoints
        playerGO->setSpawn(spawnPoints[playerGO->getUUID() % spawnPoints.size()]);
        // Insert PlayerGO into the gameObject Manager
        gameObjectManager->insert(playerGO);
        //gameObjectManager->insertClient(playerGO);
        // Sets the playerID into GameObjectManager so that it does not update incorrectly
        gameObjectManager->setPlayerID(playerGO->getUUID());

        // Convert the player game object into JSON
        json stringPrint;
        playerGO->to_json(stringPrint);
        std::cout << "String after added to gameObjectManager" << stringPrint.dump() << "\n";

        // Use the player ID to establish a unique socket for connections
        int portNum = 5558 + playerGO->getUUID();

        std::stringstream ss;
        ss << "tcp://localhost:" << portNum;

        // Set to only receive last message (theoretically)
        zmq_setsockopt(p2ppublisher, ZMQ_CONFLATE, &conflate, sizeof(conflate));
        zmq_setsockopt(p2psubscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        p2ppublisher->bind(ss.str());
        //entityController->setPlayerID(player->getUUID());

        // Set the client identifier (SECTION 5)
        subscriber->set(zmq::sockopt::subscribe, "");
        p2psubscriber->set(zmq::sockopt::subscribe, "");

        return 0;
    }

    /**
    * Run the networking communication setup
    * @param subscriber Subscriber to use
    */
    int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber,
    PlayerGO*& player, GameObjectManager*& gameObjectManager) {
        // Receive messages from the server as a subscriber for MovingEntities only
        zmq::message_t serverInfo;
        zmq_connect(subscriber, "tcp://localhost:5555");
        subscriber->recv(serverInfo, zmq::recv_flags::dontwait);
        zmq_disconnect(subscriber, "tcp://localhost:5555");

        if (!serverInfo.empty()) {

            //std::cout << "Information from the server: " << serverInfo.to_string() << "\n";

            std::stringstream ss;
            ss.clear();
            ss.str(serverInfo.to_string());

            //std::cout << "Received from Server: " << serverInfo.to_string() << "\n";

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

            // Remove clock value from serverInfo string

            std::getline(ss, firstLine);
            gameObjectManager->deserialize(firstLine, 2);
            //entityController->updateEntitiesByString(serverInfo.to_string(), 2);
        }

        // PEER TO PEER
        json stringPlayer;
        player->to_json(stringPlayer);
        zmq::message_t playerInfo("Client\n" + stringPlayer.dump());

        //std::cout << "Publishing Client Info: " << playerInfo.to_string() << "\n\n\n";

        // Iterate through opposing players
	    std::map<int, GameObject*>::iterator iter;
        std::map<int, GameObject*> playerMap = *gameObjectManager->getClientObjectMap();

		//// Receive client info
		//zmq::message_t clientInfo;
		//p2psubscriber->recv(clientInfo, zmq::recv_flags::dontwait);

		//if (!clientInfo.empty()) {
		//	// Trim the client info string
		//	std::string clientString = clientInfo.to_string().substr(clientInfo.to_string().find('\n') + 1);

		//	std::cout << "Subscriber Client Info: " << clientString << "\n\n\n\n\n";

		//	// Update the player using the string
		//	//gameObjectManager->deserialize(clientString, 2);
		//	gameObjectManager->deserializeClient(clientString, 2);
		//}

		// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
	    for (iter = playerMap.begin(); iter != playerMap.end(); ++iter) {

		    int objectId = iter->first; // Get the ID of the current entity
            //std::cout << "Hello, playerMap is populated\n";

            // Calculate port and connect with both
            int portNum = 5558 + objectId;
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
                // Trim the client info string
                std::string clientString = clientInfo.to_string().substr(clientInfo.to_string().find('\n') + 1);

                std::cout << "Subscriber Client Info: " << clientString << "\n\n\n\n\n";

                // Update the player using the string
                //gameObjectManager->deserialize(clientString, 2);
                gameObjectManager->deserializeClient(clientString, 2);
            }
        }
        return 0;
    }

    int startThread(zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, PlayerGO*& player) {

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

    int runOpposingPlayerThread() {
        // TODO: Not done yet
        return -1;
    }

    int runPlayerThread(zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, Entities::Player*& player) {
        zmq::message_t playerInfo("Client\n" + player->toString());

        //send messages
        p2ppublisher->send(playerInfo, zmq::send_flags::dontwait);

        // Receive client info
        zmq::message_t clientInfo;
        p2psubscriber->recv(clientInfo, zmq::recv_flags::dontwait);

        if (!clientInfo.empty()) {
            //std::cout << "Print peer's player info: " << clientInfo.to_string() << "\n";
            // Create player from string and update their information
            Entities::Player updatedPlayer = *Entities::Player::fromString(clientInfo.to_string());

            // TODO: Add update function for each player here

            //std::cout << "Player toString: " << updatedPlayer.toString() << "\n";
        }

        return 0;
    }
}