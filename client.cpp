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

        entityController->insertEntity((Entities::Entity)*player);
        entityController->setPlayerID(player->getUUID());

        playerController = new Controllers::PlayerController(player);

        // Set the client identifier
        //subscriber->set(zmq::sockopt::subscribe, "Client");

        // Set the client identifier (SECTION 5)
        subscriber->set(zmq::sockopt::subscribe, "Client"/*reply.to_string()*/);

        return 0;
    }

    /**
    * Run the networking communication setup
    * @param subscriber Subscriber to use
    */
    int run(zmq::socket_t* subscriber, zmq::socket_t* request, zmq::socket_t* publisher, Entities::Player player, EntityController*& entityController) {
        // Receive messages from the server as a subscriber
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result =
            zmq::recv_multipart(*(subscriber), std::back_inserter(recv_msgs), zmq::recv_flags::dontwait);
        //assert(result && "recv failed");
        //assert(*result == 2);

        if (recv_msgs.size() > 0) {
            entityController->updateEntitiesByString(recv_msgs[0].to_string());
            //std::cout << recv_msgs[0].to_string() << "\n";
        }

        //std::cout << "Player info on client: " << player.toString() << "\n";

        zmq::message_t playerInfo("Server\n" + player.toString());

        publisher->send(playerInfo, zmq::send_flags::none);

        return 0;
    }
}