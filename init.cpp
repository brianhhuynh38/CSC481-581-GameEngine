#include "definitions.h"
#include "init.h"


/* 
* Initializes SDL Image.
* Allows loading of PNGs and JPGs.
*/
//IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

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