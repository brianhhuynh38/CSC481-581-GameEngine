#pragma once

#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL.h>
#include <set>
#include <mutex>

#include "definitions.h"
#include "vector2D.h"
#include "GameObject.h"


/**
 * Display contains pointer instances to SDL_Window and SDL_Renderer.
 * This references the tutorial linked to on the SDL wiki here:
 * https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php
 */
typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
} Display;


/**
 * Collision hit responce struct. is returned when collision occurs
 */
typedef struct {
	// true if collision occured (false by default)
	bool hit;
	// directional vector of hit
	Utils::Vector2D hitVector;
	// directional vector of hit
	SDL_Rect collisionRect;
	// Collision type
	int colliderType;
	// Collided game object
	GameObject* collidedObj;
	// depth the object has penetrated into the object
	Utils::Vector2D penetrationDepth;
	// The amount that the object had moved
    Utils::Vector2D posMover;
	// The amount that the object's velocity was manipulated
	Utils::Vector2D velMover;
} HitInfo;

/**
* A set meant to contain the IDs of newly-joined clients. Includes a mutex for thread safety.
*/
typedef struct {
	// A set containing all the IDs
	std::set<int> idSet;
	// The mutex to use in the event that the queue is accessed in multiple threads
	std::mutex mutex;
} ClientIDSet;

#endif
