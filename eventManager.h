#pragma once

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "event.h"

#include <queue>

class EventManager {
private:
	// Queue of Events that are dispatched when their timestamp is less than or equal to the current time
	std::priority_queue<Events::Event> eventQueue;
public:
	/**
	* Constructor for EventManager that intializes the event queue
	*/
	EventManager();

	/**
	* Destructor for EventManager that deletes each Event stored in the manager
	*/
	~EventManager();

	/**
	* Registers an Event into the 
	*/
	void registerEvent(Events::Event *event);

	void raiseEvent();

	void dispatchEvent();
};

#endif
