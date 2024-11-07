#pragma once

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "event.h"
#include "gameObjectManager.h"

#include <queue>
#include <vector>
#include <typeindex>

class EventManager {
private:
	// Queue of Events that are dispatched when their timestamp is less than or equal to the current time
	std::priority_queue<Events::Event, std::vector<Events::Event>, std::greater<Events::Event>> m_eventQueue;
	// Maintains the history of all GameObjects that are registered to a given Event
	std::map<std::type_index, std::vector<GameObject*>> m_eventRegistry;

	// Reference to the GameObjectManager
	GameObjectManager* m_goManager;

public:
	/**
	* Constructor for EventManager that intializes the event queue
	*/
	EventManager(GameObjectManager* goManager);

	/**
	* Destructor for EventManager that deletes each Event stored in the manager
	*/
	~EventManager();

	/**
	* Registers a vector of GameObjects to a given Event
	*/
	template<typename E>
	void registerEvent(GameObject *gameObject);

	/**
	* Adds the given event into the event queue
	* @param Event to add
	*/
	void raiseEvent(Events::Event &event);

	/**
	* Dispatches any events in the priority queue which 
	* meet the time and priority requirements
	*/
	void dispatchEvents(int64_t timeStampPriority);
};

#endif
