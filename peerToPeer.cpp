#include "peerToPeer.h"

#include "configIO.h"
#include "gameObjectManager.h"
#include "structs.h"

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

        // Set conflate value to only take most recent message
        int conflate = 1;
        zmq_setsockopt(p2psubscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));
        //zmq_setsockopt(disconnectSocket, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        int portNum = 6558 + clientID;

        // Set so that it only receives the information for the specified client
        p2psubscriber.connect("tcp://localhost:" + std::to_string(portNum));
        p2psubscriber.set(zmq::sockopt::subscribe, std::to_string(clientID));

        disconnectSocket.connect("tcp://localhost:" + std::to_string(4558 + clientID)); // error

        // Set method of thread termination
        bool terminate = false;
        auto chronoEpoch = std::chrono::high_resolution_clock::now().time_since_epoch();
        int64_t timeSinceLastMessage = std::chrono::duration_cast<std::chrono::milliseconds>(chronoEpoch).count();

        //GameObject *clientGO = gameObjectManager->getClientObjectMap()->at(clientID);

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
                // Parse the json received
                //json clientJSON = json::parse(clientString);

                // Update client from JSON and set to Kinematic to lessen performance impact on current client
                /*clientGO->from_json(clientJSON);
                clientGO->getComponent<Components::RigidBody>()->setIsKinematic(true);*/
                gameObjectManager->deserializeClient(clientString, 2);
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

    int runServerThread(int clientIdentifier, GameObjectManager *gameObjectManager, std::vector<std::thread> *threads) {

        zmq::context_t context { 1 };

        zmq::socket_t serverSubscriber { context, zmq::socket_type::sub };

		// Set conflate value to only take most recent message
		int conflate = 1;
		zmq_setsockopt(serverSubscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        int portNum = 5558 + clientIdentifier;

		// Set so that it only receives the information for the specified client
		serverSubscriber.connect("tcp://localhost:" + std::to_string(portNum));
		serverSubscriber.set(zmq::sockopt::subscribe, "");

        // Whether the thread should terminate
        bool terminate = false;

        while (!terminate) {
            // Receive messages from the server as a subscriber for MovingEntities only
		    zmq::message_t serverInfo;
		    serverSubscriber.recv(serverInfo, zmq::recv_flags::dontwait);

		    if (!serverInfo.empty()) {
			    //std::cout << "Information from the server: " << serverInfo.to_string() << "\n";

				// Trim the client info string
				std::string serverString = serverInfo.to_string().substr(serverInfo.to_string().find('\n') + 1);

                // Deserialize the string into json, then insert into the objects
				gameObjectManager->deserialize(serverString, 2);
			    //std::vector<int> clientIDVector = gameObjectManager->deserialize(serverString, 2);

                //std::cout << "ClientIDVector: " << std::string(clientIDVector.begin(), clientIDVector.end());

                //// For each ID, create a new thread for running client
                //for (int id : clientIDVector) {
                //    {
                //        std::cout << "Client ID inside Client Vector: " << id << "\n"; 
                //        // Mutex here
                //        std::lock_guard<std::mutex> lock(mtx);
                //        threads->push_back(std::thread(runClientThread, id, std::ref(gameObjectManager)));
                //    }
                //    
                //}
		    }
        }

        return 0;
    }

    int runPlayerThread(PlayerGO*& player) {
        // Get player UUID
        int uuid = player->getUUID();

        zmq::context_t context { 2 };
        zmq::socket_t p2ppublisher { context, zmq::socket_type::pub };

        // Set to only receive last message (theoretically)
        int conflate = 1;
        zmq_setsockopt(p2ppublisher, ZMQ_CONFLATE, &conflate, sizeof(conflate));

        // Bind publisher to an address string
        int portNum = 5558 + uuid;
        std::string address = "tcp://localhost:" + std::to_string(uuid);
        p2ppublisher.bind(address);

        // Populate json message to send
        json stringPlayer;
        // Bool to determine if the player has terminated
        bool terminate = false;

        // Repeatedly send out player
        while (!terminate) {
            // Build json message {uuid}\n{json_string}
            player->to_json(stringPlayer);
            zmq::message_t playerInfo(std::to_string(uuid) + "\n" + stringPlayer.dump());

            // Send messages to all listeners
            p2ppublisher.send(playerInfo, zmq::send_flags::dontwait);

            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }


        return 0;
    }

    ///**
    //* Sets up client parameters and interactions
    //* @param subscriber Subscriber to setup
    //* @param request Request to setup
    //*/
    //int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber,
    //   PlayerGO*& playerGO, GameObjectManager*& gameObjectManager, ConfigSettings config, std::vector<GameObject*> spawnPoints,
    //   std::vector<std::thread>* threads) {

    //   // Set socket options
    //   int conflate = 1;
    //   zmq_setsockopt(subscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

    //   request->connect("tcp://localhost:5556");

    //   // Send message to server and wait to receive client identifier
    //   zmq::message_t msg(std::to_string(2));
    //   request->send(msg, zmq::send_flags::none);

    //   // Sleep for 2 seconds to guarantee connection
    //   std::this_thread::sleep_for(std::chrono::seconds(2));

    //   // Receive response from server 
    //   zmq::message_t reply;
    //   request->recv(reply);

    //   //std::cout << "Received client identifier: " << reply.to_string() << "\n";
    //   // TODO: Get the number from the end of the first line identifier (ex. Client_1, get 1 and store in int)
    //   int clientIdentifier = std::stoi(
    //       reply.to_string().substr(
    //           reply.to_string().find('_') + 1
    //       )
    //   );

    //   //std::cout << "Client identifier: " << clientIdentifier << "\n";

    //   // Trim the identifier off the json
    //   std::string playerString = reply.to_string().substr(reply.to_string().find('\n') + 1);

    //   // Parse the json string received from the server into playerGO
    //   json j = json::parse(playerString);
    //   playerGO->from_json(j);

    //   // Sets a spawn point based off the size of the list of SpawnPoints
    //   playerGO->setSpawn(spawnPoints[playerGO->getUUID() % spawnPoints.size()]);
    //   // Insert PlayerGO into the gameObject Manager
    //   gameObjectManager->insert(playerGO);
    //   // Sets the playerID into GameObjectManager so that it does not update incorrectly
    //   gameObjectManager->setPlayerID(playerGO->getUUID());

    //   // Create thread for player publisher and server to client communication
    //   
    //   {
    //       // Mutex here
    //       std::lock_guard<std::mutex> lock(mtx);
    //       threads->push_back(std::thread(runPlayerThread, std::ref(playerGO)));
    //       threads->push_back(std::thread(runServerThread, clientIdentifier, std::ref(gameObjectManager), std::ref(threads)));
    //   }

    //   //// Convert the player game object into JSON
    //   //json stringPrint;
    //   //playerGO->to_json(stringPrint);
    //   ////std::cout << "String after added to gameObjectManager" << stringPrint.dump() << "\n";

    //   //// Use the player ID to establish a unique socket for connections
    //   //int portNum = 5558 + clientIdentifier;
    //   //int p2pPortNum = 6558 + playerGO->getUUID();

    //   //std::stringstream ss;
    //   //ss << "tcp://localhost:" << portNum;

    //   //// Set to only receive last message (theoretically)
    //   //zmq_setsockopt(p2ppublisher, ZMQ_CONFLATE, &conflate, sizeof(conflate));
    //   //zmq_setsockopt(p2psubscriber, ZMQ_CONFLATE, &conflate, sizeof(conflate));

    //   //subscriber->connect(ss.str());

    //   //subscriber->set(zmq::sockopt::subscribe, "");
    //   //p2psubscriber->set(zmq::sockopt::subscribe, "");

    //   return 0;
    //}

    /**
    * Sets up client parameters and interactions
    * @param subscriber Subscriber to setup
    * @param request Request to setup
    */
    int startup(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber,
        PlayerGO*& playerGO, GameObjectManager*& gameObjectManager, ConfigSettings config, std::vector<GameObject*> spawnPoints,
        std::vector<std::thread> *threads) {

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
        // TODO: Get the number from the end of the first line identifier (ex. Client_1, get 1 and store in int)
        int clientIdentifier = std::stoi(
            reply.to_string().substr(
                reply.to_string().find('_') + 1
            )
        );

        //std::cout << "Client identifier: " << clientIdentifier << "\n";

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

        // Create thread for player publisher
        //threads->push_back(std::thread(runPlayerThread, std::ref(playerGO)));
      
        // Convert the player game object into JSON
        json stringPrint;
        playerGO->to_json(stringPrint);
        //std::cout << "String after added to gameObjectManager" << stringPrint.dump() << "\n";

        // Use the player ID to establish a unique socket for connections
        int portNum = 6658 + clientIdentifier;
        int p2pPortNum = 6558 + playerGO->getUUID();

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

  //  /**
  //  * Run the networking communication setup
  //  * @param subscriber Subscriber to use
  //  */
  //  int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber,
  //  PlayerGO*& player, GameObjectManager*& gameObjectManager, std::vector<std::thread>* threads) {
  //      // Receive messages from the server as a subscriber for MovingEntities only
  //      zmq::message_t serverInfo;
  //      //zmq_connect(subscriber, "tcp://localhost:5555");
  //      subscriber->recv(serverInfo, zmq::recv_flags::dontwait);
  //      //zmq_disconnect(subscriber, "tcp://localhost:5555");

  //      std::vector<int> idVector = std::vector<int>();

  //      if (!serverInfo.empty()) {
  //          //std::cout << "Information from the server: " << serverInfo.to_string() << "\n";

  //          std::stringstream ss;
  //          ss.clear();
  //          ss.str(serverInfo.to_string());

  //          //std::cout << "Received from Server: " << serverInfo.to_string() << "\n";

  //          // Read the first line to extract the clock value
  //          std::string firstLine;
  //          std::getline(ss, firstLine);

  //          //// Now, extract the clock from the first line
  //          //std::stringstream clockStream(firstLine);
  //          //int64_t clock;
  //          //if (!(clockStream >> clock)) {
  //          //    std::cerr << "Failed to parse clock value." << std::endl;
  //          //    // return -1;
  //          //}

  //          //// Calculate the delay (SUICIDE-SNAIL SOLUTION)
  //          //const auto delay = std::clock() - clock;
  //          //if (delay > MAX_ALLOWED_DELAY) {
  //          //    std::cerr << "E: subscriber cannot keep up, aborting. Delay=" << delay << std::endl;
  //          //    // return -1;
  //          //}

  //          // Remove clock value from serverInfo string

  //          std::getline(ss, firstLine);
  //          idVector = gameObjectManager->deserialize(firstLine, 2);

  //          //std::cout << "Test ID Vector: ";

  //          //for (int id : idVector) {
  //          //    std::cout << id << ", ";
  //          //}

  //          //std::cout << "\n";

  //          //entityController->updateEntitiesByString(serverInfo.to_string(), 2);
  //      }

  //      // PEER TO PEER
  //      json stringPlayer;
  //      player->to_json(stringPlayer);
  //      zmq::message_t playerInfo("Client\n" + stringPlayer.dump());

  //      std::cout << "Publishing Client Info: " << playerInfo.to_string() << "\n\n\n";

  //      // For each ID, create a new thread for running client
  //      for (int id : idVector) {
  //         {
  //             std::cout << "Client ID inside Client Vector: " << id << "\n"; 
  //             // Mutex here
  //             std::lock_guard<std::mutex> lock(mtx);
  //             //threads->push_back(std::thread(runPlayerThread, std::ref(gameObjectManager)));
  //             threads->push_back(std::thread(runClientThread, id, std::ref(gameObjectManager)));
  //         }    
  //      }

  //     // Iterate through opposing players
	 ////  std::map<int, GameObject*>::iterator iter;
  ////     std::map<int, GameObject*> playerMap = *gameObjectManager->getClientObjectMap();

		////// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
	 ////  for (iter = playerMap.begin(); iter != playerMap.end(); ++iter) {

		////   int objectId = iter->first; // Get the ID of the current entity

  ////         // Calculate port and connect with both
  ////         int portNum = 5558 + objectId;
  ////         int portNum2 = 6558 + objectId;

  ////         std::stringstream ss;
  ////         ss << "tcp://localhost:" << portNum;

  ////         std::stringstream ss2;
  ////         ss2 << "tcp://localhost:" << portNum2;

  ////         p2ppublisher->connect(ss.str());
  ////         p2psubscriber->connect(ss2.str());

  ////         //send messages
  ////         p2ppublisher->send(playerInfo, zmq::send_flags::dontwait);

  ////         // Receive client info
  ////         zmq::message_t clientInfo;
  ////         p2psubscriber->recv(clientInfo, zmq::recv_flags::dontwait);

  ////         if (!clientInfo.empty()) {
  ////             // Trim the client info string
  ////             std::string clientString = clientInfo.to_string().substr(clientInfo.to_string().find('\n') + 1);

  ////             //std::cout << "Subscriber Client Info: " << clientString << "\n\n\n\n\n";

  ////             // Update the player using the string
  ////             //gameObjectManager->deserialize(clientString, 2);
  ////             gameObjectManager->deserializeClient(clientString, 2);
  ////         }
  ////     }
  //      return 0;
  //  }

	/**
    * Run the networking communication setup
    * @param subscriber Subscriber to use
    */
    int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, PlayerGO*& player,
        GameObjectManager*& gameObjectManager, std::vector<std::thread>* threads) {

		// Receive messages from the server as a subscriber for MovingEntities only
		zmq::message_t serverInfo;
		//zmq_connect(subscriber, "tcp://localhost:5555");
		subscriber->recv(serverInfo, zmq::recv_flags::dontwait);
		//zmq_disconnect(subscriber, "tcp://localhost:5555");

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

			//// Now, extract the clock from the first line
			//std::stringstream clockStream(firstLine);
			//int64_t clock;
			//if (!(clockStream >> clock)) {
			//    std::cerr << "Failed to parse clock value." << std::endl;
			//    // return -1;
			//}

			//// Calculate the delay (SUICIDE-SNAIL SOLUTION)
			//const auto delay = std::clock() - clock;
			//if (delay > MAX_ALLOWED_DELAY) {
			//    std::cerr << "E: subscriber cannot keep up, aborting. Delay=" << delay << std::endl;
			//    // return -1;
			//}

			// Remove clock value from serverInfo string

			std::getline(ss, firstLine);
			idVector = gameObjectManager->deserialize(firstLine, 2);

            if (!idVector.empty()) {
                //std::cout << "ID Vector Results: ";
                for (int id : idVector) {
                    std::cout << id << ", ";
                }
                std::cout << "\n";
            }

			//entityController->updateEntitiesByString(serverInfo.to_string(), 2);
		}

		// PEER TO PEER
		json stringPlayer;
		player->to_json(stringPlayer);
		zmq::message_t playerInfo(std::to_string(player->getUUID()) + "\n" + stringPlayer.dump());


        int portNum = 6558 + player->getUUID();
        p2ppublisher->connect("tcp://localhost:" + std::to_string(portNum));

        // Send playerInfo
        p2ppublisher->send(playerInfo, zmq::send_flags::dontwait);

		//std::cout << "Publishing Client Info: " << playerInfo.to_string() << "\n\n\n";

        for (int id : idVector) {
            threads->push_back(std::thread(runClientThread, id, std::ref(gameObjectManager)));
        }

		// Iterate through opposing players
		//std::map<int, GameObject*>::iterator iter;
		//std::map<int, GameObject*> playerMap = *gameObjectManager->getClientObjectMap();

		//// Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
		//for (iter = playerMap.begin(); iter != playerMap.end(); ++iter) {

		//	int objectId = iter->first; // Get the ID of the current entity

		//	// Calculate port and connect with both
		//	int portNum = 5558 + objectId;
		//	int portNum2 = 6558 + objectId;

		//	std::stringstream ss;
		//	ss << "tcp://localhost:" << portNum;

		//	std::stringstream ss2;
		//	ss2 << "tcp://localhost:" << portNum2;

		//	p2ppublisher->connect(ss.str());
		//	p2psubscriber->connect(ss2.str());

		//	//send messages
		//	p2ppublisher->send(playerInfo, zmq::send_flags::dontwait);

            

			//// Receive client info
			//zmq::message_t clientInfo;
			//p2psubscriber->recv(clientInfo, zmq::recv_flags::dontwait);

			//if (!clientInfo.empty()) {
			//	// Trim the client info string
			//	std::string clientString = clientInfo.to_string().substr(clientInfo.to_string().find('\n') + 1);

			//	//std::cout << "Subscriber Client Info: " << clientString << "\n\n\n\n\n";

			//	// Update the player using the string
			//	//gameObjectManager->deserialize(clientString, 2);
			//	/*std::vector<int> idVector = */gameObjectManager->deserializeClient(clientString, 2);
			//}
		//}
		return 0;
	}

    ///**
    //* Run the networking communication setup
    //* @param subscriber Subscriber to use
    //*/
    //int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* p2ppublisher, zmq::socket_t* p2psubscriber, PlayerGO*& player,
    //    GameObjectManager*& gameObjectManager, std::vector<std::thread>* threads) {

    //    // Receive messages from the server as a subscriber for MovingEntities only
    //    zmq::message_t serverInfo;
    //    //zmq_connect(subscriber, "tcp://localhost:5555");
    //    subscriber->recv(serverInfo, zmq::recv_flags::dontwait);
    //    //zmq_disconnect(subscriber, "tcp://localhost:5555");

    //    if (!serverInfo.empty()) {
    //        //std::cout << "Information from the server: " << serverInfo.to_string() << "\n";

    //        std::stringstream ss;
    //        ss.clear();
    //        ss.str(serverInfo.to_string());

    //        //std::cout << "Received from Server: " << serverInfo.to_string() << "\n";

    //        // Read the first line to extract the clock value
    //        std::string firstLine;
    //        std::getline(ss, firstLine);

    //        //// Now, extract the clock from the first line
    //        //std::stringstream clockStream(firstLine);
    //        //int64_t clock;
    //        //if (!(clockStream >> clock)) {
    //        //    std::cerr << "Failed to parse clock value." << std::endl;
    //        //    // return -1;
    //        //}

    //        //// Calculate the delay (SUICIDE-SNAIL SOLUTION)
    //        //const auto delay = std::clock() - clock;
    //        //if (delay > MAX_ALLOWED_DELAY) {
    //        //    std::cerr << "E: subscriber cannot keep up, aborting. Delay=" << delay << std::endl;
    //        //    // return -1;
    //        //}

    //        // Remove clock value from serverInfo string

    //        std::getline(ss, firstLine);
    //        gameObjectManager->deserialize(firstLine, 2);
    //        //entityController->updateEntitiesByString(serverInfo.to_string(), 2);
    //    }

    //    // PEER TO PEER
    //    json stringPlayer;
    //    player->to_json(stringPlayer);
    //    zmq::message_t playerInfo(std::to_string(player->getUUID()) + "\n" + stringPlayer.dump());

    //    //std::cout << "Publishing Client Info: " << playerInfo.to_string() << "\n\n\n";

    //    // Iterate through opposing players
    //    std::map<int, GameObject*>::iterator iter;
    //    std::map<int, GameObject*> playerMap = *gameObjectManager->getClientObjectMap();

    //    // Updates the physics vectors for each entity in the list of entities that is tagged as "affectedByPhysics"
    //    for (iter = playerMap.begin(); iter != playerMap.end(); ++iter) {

    //        int objectId = iter->first; // Get the ID of the current entity

    //        // Calculate port and connect with both
    //        int portNum = 5558 + objectId;
    //        int portNum2 = 6558 + objectId;

    //        std::stringstream ss;
    //        ss << "tcp://localhost:" << portNum;

    //        std::stringstream ss2;
    //        ss2 << "tcp://localhost:" << portNum2;

    //        p2ppublisher->connect(ss.str());
    //        p2psubscriber->connect(ss2.str());

    //        //send messages
    //        p2ppublisher->send(playerInfo, zmq::send_flags::dontwait);

    //        // Receive client info
    //        zmq::message_t clientInfo;
    //        p2psubscriber->recv(clientInfo, zmq::recv_flags::dontwait);

    //        if (!clientInfo.empty()) {
    //            // Trim the client info string
    //            std::string clientString = clientInfo.to_string().substr(clientInfo.to_string().find('\n') + 1);

    //            //std::cout << "Subscriber Client Info: " << clientString << "\n\n\n\n\n";

    //            // Update the player using the string
    //            //gameObjectManager->deserialize(clientString, 2);
    //            gameObjectManager->deserializeClient(clientString, 2);
    //        }
    //    }
    //    return 0;
    //}
    
}