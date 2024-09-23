#include <SDL.h>
#include <list>
#include "definitions.h"
#include "vector2D.h"
#pragma once

/**
 * Handles logic and draw functions for the main game loop.
 * This references the tutorial linked to on the SDL wiki here :
 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
 */
typedef struct {
	void (*logic)(void);
	void (*draw)(void);
} Delegate;

/**
 * Display contains pointer instances to SDL_Window and SDL_Renderer.
 * This references the tutorial linked to on the SDL wiki here:
 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
 */
typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Delegate delegate;
} Display;

/**
 * 
 * This references the tutorial linked to on the SDL wiki here :
 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter5.php
 */
// typedef struct {
// 	int map[MAP_WIDTH][MAP_HEIGHT];
// 	std::list<Entities::Player> m_players;
// 	std::list<Entities::Entity> m_entities;
// } Stage;

/**
 * InputHandler holds the current input values for keypresses.
 */
typedef struct {
	// list of all available keys and their states
	int keyboard[MAX_KEYBOARD_KEYS];
} InputHandler;

/**
 * Collision hit responce struct. is returned when collision occurs
 */
typedef struct {
	// true if collision occured (false by default)
	bool hit;
	// directional vector of hit
	Utils::Vector2D hitVector;
} HitInfo;

/**
 * Struct to store player information
 */
typedef struct {
	float x, y; // Position coordinates
	float vx, vy; // Velocity
} PlayerInfo;