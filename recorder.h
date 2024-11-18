#pragma once

#ifndef RECORDER_H
#define RECORDER_H

#include "event.h"
#include "gameObject.h"

#include <map>
#include <vector>

class Recorder {  
private:
	// vector list of events dispatched during recording
	std::vector<Events::Event> eventHistory;
	// Map of position for each gameObject (UUID) at the start of recording
	std::map<int, Utils::Vector2D> startPositions;
	// Map of positons for each gameObject (UUID) prior to beginning a playback
	std::map<int, Utils::Vector2D> tempSnapshotPositions;
	int64_t startingTimeStamp;
	int64_t endingTimeStamp;
public:
	/**
	* Recorder constructor
	*/
	Recorder();
};

#endif 
