#pragma once
#include "structs.h"
#include "stage.h"

static class StageManager {
private:
	/** CurrentStage index to display */
	int currStageIndex;
	/** Array of stages */
	Stages::Stage stages[];
public:

	/**
	 * StageManagerConstructor
	 */
	 StageManager();

	 void loadAllMaps(char* filepath);

	 char* readFile(const char* filename);

	 Stages::Stage getMap(int index);
	/*
	* Initializes the map, holding the map data information
	* This references the tutorial linked to on the SDL wiki here:
	* https://www.parallelrealities.co.uk/tutorials/ppp/ppp1.php
	*/
	 void initMap(void);

	 void initStage(void);
};
