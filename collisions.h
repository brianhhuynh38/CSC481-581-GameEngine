#include <SDL.h>
#include <iostream>

#include "input.h"
#include "structs.h"
#include "global.h"

#pragma once
#ifndef COLLISIONS_H
#define COLLISIONS_H

/**
 * Checks collisions
 * This was referenced from the SDL2 tutorial:
 * https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
 */
bool checkCollision(SDL_Rect a, SDL_Rect b);

/**
 * Loop through entities, use their colliders to check the collisions, using checkCollision method
 * This was referenced from the SDL2 tutorial:
 * https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
 */
bool checkCollisions(std::list<SDL_Rect> collisions, std::list<Entities::Entity> entities);

#endif