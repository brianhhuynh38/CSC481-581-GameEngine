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

/** Whether or not this client is recording their events */
extern bool isRecording;
/** Whether or not a playback is in process */
extern bool startPlayback;

// The global instance of eventManager that everything has access to
extern EventManager* eventManager;


#endif
