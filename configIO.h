#pragma once

#ifndef CONFIGIO_H
#define CONFIGIO_H

// Stores configuration settings
typedef struct ConfigSettings {
	/// Gravity Value
	float gravity;
	/// The resolution width the window is set to
	int resolutionWidth;
	/// The resolution Height the window is set to
	int resolutionHeight;
	/// Network Type: 1 = Client to Server, 2 = Peer to Peer
	int networkType;
};


/**
* Loads variables defined in the config.ini file and sets them to their proper places
*/
void loadConfigFile(ConfigSettings *settings);


#endif
