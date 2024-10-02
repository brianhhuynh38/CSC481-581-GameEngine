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

namespace Client {
    // Map to store players by their ID from the client
    std::unordered_map<std::string, PlayerInfo> players;
    // A timer used to delay the message being sent (A temporary measure because ZMQ_LINGER seems
    // to be deprecated with the version we're using (4.3.5))
    // In retrospect, maybe shoulda used 4.3.2
    int messageDelayCounter = 0;

    /**
    * Sets up client parameters and interactions
    * @param subscriber Subscriber to setup
    * @param request Request to setup
    */
    int startup(zmq::socket_t *subscriber, zmq::socket_t *request, zmq::socket_t *publisher, Entities::Player *&player, EntityController *&entityController, Controllers::PlayerController *&playerController) {
        subscriber->connect("tcp://localhost:5555");
        request->connect("tcp://localhost:5556");
        publisher->connect("tcp://localhost:5557");

        // Send message to server and wait to receive client identifier
        request->send(zmq::str_buffer("Requesting client identifier"));

        // Sleep for 2 seconds to guarantee connection
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Receive response from server 
        zmq::message_t reply;
        request->recv(reply, zmq::recv_flags::none);
        //std::cout << "Received client identifier: " << reply.to_string() << "\n";

        // Should continue from after uuid
        std::string playerString = reply.to_string();
        //playerString.erase(0, playerString.find("\n") + 1);

        //std::cout << "Printing fresh playerstring off server: \n" << playerString << "\n";

        player = Entities::Player::fromString(playerString);

        //std::cout << "FULLY CREATED ENTITY ON CLIENT: \n" << player->toString() << "\n";
        entityController->setPlayerID(player->getUUID());
        entityController->insertEntity((Entities::Entity)*player);

        playerController = new Controllers::PlayerController(player);

        // Set the client identifier
        //subscriber->set(zmq::sockopt::subscribe, "Client");

        // Set the client identifier (SECTION 5)
        subscriber->set(zmq::sockopt::subscribe, "");

        return 0;
    }

    /**
    * Run the networking communication setup
    * @param subscriber Subscriber to use
    */
    int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* publisher, Entities::Player *&player, EntityController*& entityController) {
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

            // Now, extract the clock from the first line
            std::stringstream clockStream(firstLine);
            int64_t clock;
            if (!(clockStream >> clock)) {
                std::cerr << "Failed to parse clock value." << std::endl;
                return -1;
            }

            // Calculate the delay (SUICIDE-SNAIL SOLUTION)
            const auto delay = std::clock() - clock;
            if (delay > MAX_ALLOWED_DELAY) {
                std::cerr << "E: subscriber cannot keep up, aborting. Delay=" << delay << std::endl;
                return -1;
            }

            entityController->updateEntitiesByString(serverInfo.to_string());
        }

        if (messageDelayCounter >= ZMQ_MSG_DELAY) {
            std::cout << "Sending message...\n";
            //std::cout << "Player info to send to server: " << player->toString() << "\n";
            zmq::message_t playerInfo("Server\n" + player->toString());
            publisher->send(playerInfo, zmq::send_flags::dontwait);
            messageDelayCounter = 0;
        }
        
        messageDelayCounter++;

        return 0;
    }
}