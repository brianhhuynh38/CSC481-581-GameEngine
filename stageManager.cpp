#include "structs.h"

#include "stageManager.h"


/**
 * StageManager Constructor
 */
StageManager::StageManager() {

}

void StageManager::loadAllMaps(char* filepath) {
	// Loop through maps in the filepath, call readFile
}

char * StageManager::readFile(const char *filename) {
	/*char *buffer = 0;
	unsigned long length;
	FILE *file = fopen(filename, "rb");

	if (file) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);
			
		buffer = malloc(length + 1);
		memset(buffer, 0, length + 1);
		fread(buffer, 1, length, file);
			
		fclose(file);

		buffer[length] = '\0';
	}*/
	return NULL;
}

Stages::Stage StageManager::getMap(int index) {
	return stages[index];
}

/*
* Initializes the map, holding the map data information
* This references the tutorial linked to on the SDL wiki here:
* https://www.parallelrealities.co.uk/tutorials/ppp/ppp1.php
*/
void StageManager::initMap(void) {
	/*memset(&stage.map, 0, sizeof(int) * MAP_WIDTH * MAP_HEIGHT);

	loadTiles();

	loadMap("data/map01.dat");*/
}

void StageManager::initStage(void)
{
	/* display.delegate.logic = logic;
		display.delegate.draw = draw;*/

	initMap();
}