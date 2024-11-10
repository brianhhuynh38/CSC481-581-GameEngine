#include "peerToPeer.h"

#include "configIO.h"
#include "gameObjectManager.h"

#include "structs.h"
#include "global.h"

#include "playerUpdateEvent.h"
#include "moveObjectEvent.h"
#include "instantiateObjectEvent.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include <chrono>
#include <thread>
#include <mutex>

#include <zmq.hpp>
#include <zmq_addon.hpp>

namespace PeerToPeer {

    std::mutex mtx;

    int runClientThread(int clientID, GameObjectManager* gameObjectManager) {

        // Create the context for this thread
        zmq::context_t context{ 4 };
        // Create a subscriber using a Sub model
        zmq::socket_t p2psubscriber{ context, zmq::socket_type::sub };
        zmq::socket_t disconnectSocket{ context, zmq::socket_type::req };
        // Create socket references for use in Events


        // Set conflate value to only take most recent message
        int conflate = 1;
        zmq_setsockopt(p2psubscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        int portNum = 6558 + clientID;

        // Set so that it only receives the information for the specified client
        p2psubscriber.connect("tcp://localhost:" + std::to_string(portNum));
        p2psubscriber.set(zmq::sockopt::subscribe, std::to_string(clientID));

        disconnectSocket.connect("tcp://localhost:" + std::to_string(4558 + clientID)); // error

        // Set method of thread termination
        bool terminate = false;
        auto chronoEpoch = std::chrono::high_resolution_clock::now().time_since_epoch();
        int64_t timeSinceLastMessage = std::chrono::duration_cast<std::chrono::milliseconds>(chronoEpoch).count();

        while (!terminate) {

            // Receive client information
            zmq::message_t clientInfo;
            p2psubscriber.recv(clientInfo, zmq::recv_flags::dontwait);

            // If it is populated, then use to populate updated fields
            if (!clientInfo.empty()) {

                // Update the time since last message was received
                chronoEpoch = std::chrono::high_resolution_clock::now().time_since_epoch();
                timeSinceLastMessage = std::chrono::duration_cast<std::chrono::milliseconds>(chronoEpoch).count();

                // Trim the client info string
                std::string clientString = clientInfo.to_string().substr(clientInfo.to_string().find('\n') + 1);

                // Raise event to handle player updates (might change to use client queues)
                eventManager->raiseEventInstantly(new Events::PlayerUpdateEvent(gameObjectManager, 0, 0, clientString));

                //// Update client from JSON and set to Kinematic to lessen performance impact on current client
                //gameObjectManager->deserializeClient(clientString, 2);

                

            }
            else { // If not, keep track of time that the thread does not receive messages; after 10 seconds: terminate

                // Get current time
                auto timeCheckEpoch = std::chrono::high_resolution_clock::now().time_since_epoch();
                int64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeCheckEpoch).count();

                // Terminate if 5 seconds or more have elapsed without any messages
                if (currentTime - timeSinceLastMessage >= 5000 and gameObjectManager->getClientObjectMap()->count(clientID)) {
                    std::cout << "\n\n\nTry to terminate client " << clientID << "\n\n\n";

                    // Send client UUID to the server to terminate
                    zmq::message_t disconnectMessage(std::to_string(clientID));
                    disconnectSocket.send(disconnectMessage, zmq::send_flags::none);
                    std::cout << "\n\n\nTerminated Other Client at PlayerID " << clientID << "\n\n\n";

                    zmq::message_t reply;
                    disconnectSocket.recv(reply);

                    if (!reply.empty()) {
                        gameObjectManager->terminateClient(clientID);
                        terminate = true;
                    }
                }
            }
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
        return 0;
    }

    /**
    * Sets up client parameters and interactions
    * @param subscriber Subscriber to setup
    * @param request Request to setup
    */
    int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber,
        GameObjectManager*& gameObjectManager, ConfigSettings config, std::vector<GameObject*> spawnPoints, std::vector<std::thread> *threads) {

        // Set socket options
        int conflate = 1;
        zmq_setsockopt(subscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        request->connect("tcp://localhost:5556");
      
        // Send message to server and wait to receive client identifier
        zmq::message_t msg(std::to_string(2));
      
        request->send(msg, zmq::send_flags::none);

        // Sleep for 2 seconds to guarantee connection
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Receive response from server 
        zmq::message_t reply;
        request->recv(reply);

        //std::cout << "Received client identifier: " << reply.to_string() << "\n";

        int clientIdentifier = std::stoi(
            reply.to_string().substr(
                reply.to_string().find('_') + 1
            )
        );

        //std::cout << "Client identifier: " << clientIdentifier << "\n";

        // Trim the identifier off the json
        std::string starterInfoString = reply.to_string().substr(reply.to_string().find('\n') + 1);

        //std::cout << "Printing fresh playerstring off server: \n" << playerString << "\n\n\n";

        // Parse the json string received from the server into playerGO
        json j = json::parse(starterInfoString);

        // Raises an event that should be taking information in regarding all existing network objects and defines the player
        eventManager->raiseEventInstantly(new Events::InstantiateObjectEvent(gameObjectManager, 0, 0, j["gos"].dump(), j["playerid"].get<int>()));

        // Create thread for player publisher
        //threads->push_back(std::thread(runPlayerThread, std::ref(playerGO)));
      
        // Convert the player game object into JSON
        json stringPrint;

        PlayerGO* player = gameObjectManager->tryGetPlayer();
        player->to_json(stringPrint);

        // Sets a spawn point based off the size of the list of SpawnPoints
        player->setSpawn(spawnPoints[player->getUUID() % spawnPoints.size()]);

        // Use the player ID to establish a unique socket for connections
        int portNum = 6658 + clientIdentifier;
        int p2pPortNum = 6558 + player->getUUID();

        std::stringstream ss;
        ss << "tcp://localhost:" << portNum;

        std::stringstream p2pss;
        p2pss << "tcp://localhost:" << p2pPortNum;

        // Set to only receive last message (theoretically)
        zmq_setsockopt(p2ppublisher, ZMQ_CONFLATE, &conflate, sizeof(conflate));
        zmq_setsockopt(p2psubscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        subscriber->connect(ss.str());
        p2ppublisher->bind(p2pss.str());

        // Set the client identifier (SECTION 5)
        subscriber->set(zmq::sockopt::subscribe, "");
        p2psubscriber->set(zmq::sockopt::subscribe, "");

        return 0;
    }

	/**
    * Run the networking communication setup
    * @param subscriber Subscriber to use
    */
    int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, PlayerGO*& player,
        GameObjectManager*& gameObjectManager, std::vector<std::thread>* threads, std::queue<int>* clientIDQueue) {

		// Receive messages from the server as a subscriber for MovingEntities only
		zmq::message_t serverInfo;
		subscriber->recv(serverInfo, zmq::recv_flags::dontwait);

        std::vector<int> idVector;

		if (!serverInfo.empty()) {
			//std::cout << "Information from the server: " << serverInfo.to_string() << "\n";

			std::stringstream ss;
			ss.clear();
			ss.str(serverInfo.to_string());

			//std::cout << "Received from Server: " << serverInfo.to_string() << "\n";

			// Read the first line to extract the clock value
			std::string firstLine;
			std::getline(ss, firstLine);

			std::getline(ss, firstLine);

            // Call moveEvent for when an object moves
            eventManager->raiseEvent(new Events::MoveObjectEvent(gameObjectManager, gameObjectManager->getCurrentTime(), 1, firstLine, clientIDQueue));
		}

        // Connect port number for this client and create socket ref for events
        //int portNum = 6558 + player->getUUID();
        //p2ppublisher->connect("tcp://localhost:" + std::to_string(portNum));
        zmq::socket_ref p2pPubRef = *p2ppublisher;

        // Call playerUpdateEvent with the p2ppublisher socket
        std::vector<GameObject*> goVec = std::vector<GameObject*>();
        goVec.push_back(player);
        eventManager->raiseEvent(new Events::PlayerUpdateEvent(goVec, 0, 0, p2pPubRef, player->getUUID()));

        while (!clientIDQueue->empty()) {
            threads->push_back(std::thread(runClientThread, clientIDQueue->front(), std::ref(gameObjectManager)));
            clientIDQueue->pop();
        }

		return 0;
	}
}