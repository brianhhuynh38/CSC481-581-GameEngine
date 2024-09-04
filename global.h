#include "structs.h"
#include "vector2D.h"
#include "timeline.h"
#include "physics.h"
#include "player.h"
#include "entityController.h"

#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

/** The struct that maintains the SDL Window and Renderer */
extern Display* display;
/** The ratio used to globally scale all entities */
extern Utils::Vector2D globalScaling;
/** Handles control inputs to move the player */
extern InputHandler inputHandler;
/** The timeline used to manage time used for physics calculations */
extern Timeline timeline;
/** The space where physics calculations are done in relations to Entities*/
extern Physics physics;
/** Enables entity physics and logic */
extern EntityController entityController;

// Temporary: player entity
extern Entities::Player player;

#endif
