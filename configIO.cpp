#include <fstream>
#include <string>

#include "global.h"
#include "configIO.h"

void loadConfigFile() {
	// File to open
	std::ifstream file;
	// Open config.ini
	file.open("./config.ini", std::fstream::in);
	if (file.is_open()) {
		// Read through each line
		std::string identifier, value;
		while (std::getline(file, identifier, '=')) {
			std::getline(file, value);
			// If gravity, convert to float and set gravity
			if (identifier == "gravity") {
				physics.setGravity(std::stof(value));
			}
		}
	}
	file.close();
}

