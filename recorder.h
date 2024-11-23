#pragma once

#ifndef RECORDER_H
#define RECORDER_H

#include "event.h"
#include "gameObject.h"

#include <map>
#include <queue>

class Recorder {  
private:
	// vector list of events dispatched during recording
	std::queue<Events::Event*> m_eventHistory;
	// Map of position for each gameObject (UUID) at the start of recording
	std::map<int, Utils::Vector2D> m_startPositions;
	// Map of positons for each gameObject (UUID) prior to beginning a playback
	std::map<int, Utils::Vector2D> m_tempSnapshotPositions;
	// Int of starting time of recording
	int64_t m_startingTimeStamp;
	// Int of ending time of recording
	int64_t m_endingTimeStamp;
	// Time at which the time is offset to determine time by which the events execute
	int64_t m_startPlaybackTimeOffset;

public:
	/**
	* Recorder constructor
	*/
	Recorder();

	/**
	* Enables the recording bool that allows for recording events
	*/
	void enableRecording();

	/**
	* Disables the recording bool that allows for recording events 
	*/
	void disableRecording(); 

	/**
	*  Tries to add given event 
	*/
	void tryRecordEvent(Events::Event* event);

	void tryDispatchRecording(int64_t timeStamp);
};

#endif 
