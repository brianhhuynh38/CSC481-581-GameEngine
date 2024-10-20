#include <fstream>
#include <string>

#include "global.h"
#include "configIO.h"

/**
* Loads variables defined in the config.ini file and sets them to their proper places
*/
void loadConfigFile(void) {

	// File to open
	std::ifstream file;
	// Open config.ini
	file.open("./config.ini", std::fstream::in);
	if (file.is_open()) {
		// Read through each line
		std::string identifier, value;
		while (std::getline(file, identifier, '=')) {
			std::getline(file, value);
			std::cout << "Config Value" << value << "\n";
			// If gravity, convert to float and set gravity
			if (identifier == "gravity") {
				physics.setGravity(std::stof(value));
			}
			else if (identifier == "resolutionWidth") {
				std::cout << value;
				resolutionWidth = std::stoi(value);
			}
			else if (identifier == "resolutionHeight") {
				std::cout << value;
				resolutionHeight = std::stoi(value);
			}
		}
	}
	file.close();
}

