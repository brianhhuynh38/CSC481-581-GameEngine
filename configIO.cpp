#include <fstream>
#include <string>

#include "global.h"
#include "configIO.h"

/**
* Loads variables defined in the config.ini file and sets them to their proper places
*/
void loadConfigFile(ConfigSettings *settings) {

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
				std::cout << "Gravity: " << value << "\n";
				settings->gravity = std::stof(value);
			} // Get resolution width
			else if (identifier == "resolutionWidth") {
				std::cout << value;
				settings->resolutionWidth = std::stoi(value);
			} // Get resolution height
			else if (identifier == "resolutionHeight") {
				std::cout << value;
				settings->resolutionHeight = std::stoi(value);
			}
			else if (identifier == "networkType") {
				std::cout << "Network Type: " << value << "\n";
				settings->networkType = std::stoi(value);
			}
		}
	}
	file.close();
}

