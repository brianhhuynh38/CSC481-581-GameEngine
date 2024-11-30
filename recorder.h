#pragma once

#ifndef RECORDER_H
#define RECORDER_H

#include "event.h"

#include <map>
#include <queue>

class GameObjectManager;

class Recorder {  
private:
	// vector list of events dispatched during recording
	std::queue<Events::Event*> m_eventHistory;
	// Serialized string of all gameObject info at the start of recording
	std::string m_startPositions;

	// Int of starting time of recording
	int64_t m_startingTimeStamp;
	// Int of ending time of recording
	int64_t m_endingTimeStamp;
	// Time at which the time is offset to determine time by which the events execute
	int64_t m_startPlaybackTimeOffset;

	// Game object manager reference
	GameObjectManager *m_goManager;

public:
	/**
	* Recorder constructor
	*/
	Recorder(GameObjectManager *goManager);

	/**
	*  Tries to add given event 
	*/
	void tryRecordEvent(Events::Event* event);

	/**
	*  Tries to dispatch recorded events
	*/
	void tryDispatchRecording(int64_t timeStamp);
};

#endif 
