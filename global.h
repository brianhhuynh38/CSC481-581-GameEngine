#include "structs.h"
#include "vector2D.h"
#include "timeline.h"
#include "eventManager.h"

#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

/** The struct that maintains the SDL Window and Renderer */
extern Display* display;

/** The ratio used to globally scale all entities */
extern Utils::Vector2D globalScaling;
/** Dictates whether proportional scaling is active */
extern bool proportionalScalingActive;

// The global instance of eventManager that everything has access to
extern EventManager* eventManager;


#endif
