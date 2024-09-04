#include "structs.h"
#include "vector2D.h"
#include "timeline.h"
#include "physics.h"
#include "player.h"
#include "entityController.h"

#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

extern Display* display;
extern Utils::Vector2D globalScaling;
extern InputHandler inputHandler;
extern Timeline timeline;
extern Physics physics;
extern EntityController *entityController;

// Temporary
extern Entities::Player *player;

#endif
