#include "client.h"

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

namespace Client {
    // Currently DEPRECATED

    /**
    * Sets up client parameters and interactions
    * @param subscriber Subscriber to setup
    * @param request Request to setup
    */
    int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* publisher,
        PlayerGO*& playerGO, GameObjectManager*& gameObjectManager, ConfigSettings config, std::vector<GameObject*> spawnPoints) {
        
        request->connect("tcp://localhost:5556");
        
        // Send message to server and wait to receive client identifier
        zmq::message_t msg(std::to_string(1));

        request->send(msg, zmq::send_flags::none);

        // Sleep for 2 seconds to guarantee connection
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Receive response from server 
        zmq::message_t reply;
        request->recv(reply);

        //std::cout << "Received client identifier: " << reply.to_string() << "\n";
        // TODO: Get the number from the end of the first line identifier (ex. Client_1, get 1 and store in int)
        int clientIdentifier = std::stoi(
            reply.to_string().substr(
                reply.to_string().find('_') + 1
            )
        );

       // std::cout << "Client identifier: " << clientIdentifier << "\n";

        // Trim the identifier off the json
        std::string playerString = reply.to_string().substr(reply.to_string().find('\n') + 1);

        //std::cout << "Printing fresh playerstring off server: \n" << playerString << "\n\n\n";

        // Parse the json string received from the server into playerGO
        json j = json::parse(playerString);
        playerGO->from_json(j);

        // Sets a spawn point based off the size of the list of SpawnPoints
        playerGO->setSpawn(spawnPoints[playerGO->getUUID() % spawnPoints.size()]);
        // Insert PlayerGO into the gameObject Manager
        gameObjectManager->insert(playerGO);
        // Sets the playerID into GameObjectManager so that it does not update incorrectly
        gameObjectManager->setPlayerID(playerGO->getUUID());

        // Convert the player game object into JSON
        json stringPrint;
        playerGO->to_json(stringPrint);
        //std::cout << "String after added to gameObjectManager" << stringPrint.dump() << "\n";

        // Use the player ID to establish a unique socket for connections
        int portNum = 5558 + clientIdentifier;
        int portNum2 = 6558 + clientIdentifier;
        std::stringstream ss;
        ss << "tcp://localhost:" << portNum;
        std::stringstream ss2;
        ss2 << "tcp://localhost:" << portNum2;

        int conflate = 1;
        int linger = 0;
        zmq_setsockopt(subscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));
        zmq_setsockopt(subscriber, ZMQ_LINGER, &linger, sizeof(linger));

        subscriber->connect(ss.str());
        publisher->connect(ss2.str());

        //std::cout << "Server bound to ports: " << portNum << " and " << portNum2 << "\n";

		subscriber->set(zmq::sockopt::subscribe, "");

        return 0;
    }

    /**
    * Run the networking communication setup
    * @param subscriber Subscriber to use
    */
    int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* publisher, PlayerGO*& player,
        GameObjectManager*& gameObjectManager) {
        // Receive messages from the server as a subscriber
        zmq::message_t serverInfo;
        subscriber->recv(serverInfo, zmq::recv_flags::dontwait);

        if (!serverInfo.empty()) {
            std::stringstream ss;
            ss.clear();
            ss.str(serverInfo.to_string());

            // Read the first line to extract the clock value
            std::string firstLine;
            std::getline(ss, firstLine);

            std::getline(ss, firstLine);
            gameObjectManager->deserialize(firstLine, 1);
        }

        // CLIENT-SERVER
        json stringPlayer;
        player->to_json(stringPlayer);
        //std::cout << "Translated Player: " << stringPlayer.dump() << "\n";
        zmq::message_t playerInfo(stringPlayer.dump());

        //std::cout << "PUB TO SERVER: " << playerInfo.to_string() << "\n";
        publisher->send(playerInfo, zmq::send_flags::dontwait);

       

        return 0;
    }
}