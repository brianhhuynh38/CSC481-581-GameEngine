#include <SDL.h>
#include <iostream>
#include <sstream>

#include "network.h"
#include "structs.h"
#include "global.h"
#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if (defined (WIN32))
#include <zhelpers.hpp>
#endif



namespace Network {
	/**
	 * Handles server networking (from 0MQ tutorial)
	 */
	void server() {
        //  Prepare our context and publisher
        zmq::context_t context(1);
        zmq::socket_t publisher(context, zmq::socket_type::pub);
        publisher.bind("tcp://*:5556");
        publisher.bind("ipc://weather.ipc");				// Not usable on Windows.

        //  Initialize random number generator
        srand((unsigned)time(NULL));
        while (1) {

            int zipcode, temperature, relhumidity;

            //  Get values that will fool the boss
            zipcode = within(100000);
            temperature = within(215) - 80;
            relhumidity = within(50) + 10;

            //  Send message to all subscribers
            zmq::message_t message(20);
            snprintf((char*)message.data(), 20,
                "%05d %d %d", zipcode, temperature, relhumidity);
            publisher.send(message, zmq::send_flags::none);

        }
	}

	/**
	 * Handles client networking (from 0MQ tutorial)
	 */
	void client(int argc, char* argv[]) {
        zmq::context_t context(1);

        //  Socket to talk to server
        std::cout << "Collecting updates from weather server...\n" << std::endl;
        zmq::socket_t subscriber(context, zmq::socket_type::sub);
        subscriber.connect("tcp://localhost:5556");

        //  Subscribe to zipcode, default is NYC, 10001
        const char* filter = (argc > 1) ? argv[1] : "10001 ";
        subscriber.set(zmq::sockopt::subscribe, filter);

        //  Process 100 updates
        int update_nbr;
        long total_temp = 0;
        for (update_nbr = 0; update_nbr < 100; update_nbr++) {

            zmq::message_t update;
            int zipcode, temperature, relhumidity;

            subscriber.recv(update, zmq::recv_flags::none);

            std::istringstream iss(static_cast<char*>(update.data()));
            iss >> zipcode >> temperature >> relhumidity;

            total_temp += temperature;
        }
        std::cout << "Average temperature for zipcode '" << filter
            << "' was " << (int)(total_temp / update_nbr) << "F"
            << std::endl;
	}
}