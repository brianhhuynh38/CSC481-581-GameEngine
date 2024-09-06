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
/** Dictates whether proportional scaling is active */
extern bool proportionalScalingActive;
/* The resolution width the window is set to */
extern int resolutionWidth;
/* The resolution Height the window is set to */
extern int resolutionHeight;

/** Handles control inputs to move the player */
extern InputHandler inputHandler;
/** The timeline used to manage time used for physics calculations */
extern Timeline timeline;
/** The space where physics calculations are done in relations to Entities */
extern Physics physics;
/** controls entity logic */
extern EntityController *entityController;



// Temporary
extern Entities::Player *player;


#endif
