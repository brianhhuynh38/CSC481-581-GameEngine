#include <SDL.h>

#include "definitions.h"
#include "init.h"
#include "structs.h"

/*
* Loads the tiles.
* This references the tutorial linked to on the SDL wiki here:
* https://www.parallelrealities.co.uk/tutorials/ppp/ppp1.php
*/
static void loadTiles(void)
{
    /*int i;
    char filename[MAX_FILENAME_LENGTH];

    for (i = 1 ; i < MAX_TILE_TYPES ; i++) {
        sprintf(filename, "gfx/tile%d.png", i);

        tiles[i] = loadTexture(filename);
    }*/
}

/*
* Initializes the map, holding the map data information
* This references the tutorial linked to on the SDL wiki here:
* https://www.parallelrealities.co.uk/tutorials/ppp/ppp1.php
*/
static void loadMap(const char *filename)
{
    /*char *data, *p;
    int x, y;

    data = readFile(filename);
    p = data;

    for (y = 0 ; y < MAP_HEIGHT ; y++) {
        for (x = 0 ; x < MAP_WIDTH ; x++) {
            sscanf(p, "%d", &stage.map[x][y]);

            do {p++;} while (*p != ' ' && *p !\ '\n');
        }
    }*/
}

/*
* Draws the map, looping through the rows and columns, and showing the relevant tiles
* This references the tutorial linked to on the SDL wiki here:
* https://www.parallelrealities.co.uk/tutorials/ppp/ppp1.php
*/
void drawMap(void)
{
    /*int x, y, n;

    for (y = 0 ; y < MAP_RENDERER_HEIGHT ; y++)
    {
        for (x = 0 ; x < MAP_RENDERER_HEIGHT ; y++)
        {
            n = stage.map[x][y];

            if (n > 0)
            {
                blit(tiles[n], x * TILE_SIZE, y * TILE_SIZE, 0);
            }
        }
    }*/
}