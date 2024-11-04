#pragma once

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "event.h"

#include <queue>
#include <vector>

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
	void registerEvent();

	/**
	* Adds the given event into the event queue
	* @param Event to add
	*/
	void raiseEvent(Events::Event);

	/**
	* dispatches any events in the priority queue which 
	* meet the time and priority requirements
	*/
	void dispatchEvent();
};

#endif
