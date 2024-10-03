#pragma once

// The default resolution values defined in HW 1 for a 16:9 aspect ratio
#define DEFAULT_SCALING_WIDTH 1280
#define DEFAULT_SCALING_HEIGHT 720

// Definitions for aspect ratio width and height
#define ASPECT_W 16
#define ASPECT_H 9

// Time-related constants
#define MICROSEC_PER_SEC 1000000.0

// Max number of tile types
#define MAX_TILE_TYPES 8

// Height and width of tile
#define TILE_SIZE 64

#define MAP_WIDTH 40
#define MAP_HEIGHT 20

#define MAP_RENDER_WIDTH 20
#define MAP_RENDER_HEIGHT 12

// max keyboard keys available to press
#define MAX_KEYBOARD_KEYS 350

#define MAX_ALLOWED_DELAY 1000 // msecs

#define ZMQ_MSG_DELAY 5 // Temporary measure for zmq message overload