#pragma once

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "event.h"
//#include "gameObject.h"

#include <queue>
#include <vector>
#include <typeindex>

class GameObject;

class EventManager {
private:
	// Queue of Events that are dispatched when their timestamp is less than or equal to the current time
	// PriorityQueue<Event, Storage container, Greatest to least/least to greatest>
	std::priority_queue<Events::Event*, std::vector<Events::Event*>, std::greater<Events::Event*>> m_eventQueue;
	// Maintains the history of all GameObjects that are registered to a given Event
	std::map<std::type_index, std::vector<GameObject*>> m_eventRegistry;

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
	* Registers a vector of GameObjects to a given Event
	*/
	template <typename E>
	void registerEvent(GameObject *gameObject);

	/**
	* Adds the given event into the event queue
	* @param Event to add
	*/
	void raiseEvent(Events::Event* event);

	/**
	* Adds the given event into the event queue
	* @param timeStampPriority: The time that the event should be executed
	* @param priority: The priority of each event outside of the time priority
	*/
	template <typename E>
	void raiseEventAll(int64_t timeStampPriority, int priority);

	/**
	* Dispatches any events in the priority queue which 
	* meet the time and priority requirements
	*/
	void dispatchEvents(int64_t timeStampPriority);
};

#endif
