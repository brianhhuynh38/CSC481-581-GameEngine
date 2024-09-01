#include "definitions.h"
#include "init.h"


/*
* Initializes the map, holding the map data information
* This references the tutorial linked to on the SDL wiki here:
* https://www.parallelrealities.co.uk/tutorials/ppp/ppp1.php
*/
void initMap(void){
    /*memset(&stage.map, 0, sizeof(int) * MAP_WIDTH * MAP_HEIGHT);

    loadTiles();

    loadMap("data/map01.dat");*/
}

void initStage(void)
{
   /* display.delegate.logic = logic;
    display.delegate.draw = draw;*/

    initMap();
}