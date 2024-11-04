#pragma once

#ifndef EVENT_H
#define EVENT_H

#include "GameObject.h"
#include "gameObjectManager.h"

struct Event {
	// Reference to the GameObject that this Event should be attached to
	GameObject *goRef;

	// A Timestamp value that is used to determine when the Event should be dispatched
	int timeStampPriority;
	// The priority integer value used to help sort the priority queue alongside the Timestamp
	int priority;

	/**
	* Constructor for the Event struct
	* 
	* @param goRef: A reference to the Gameobject that the Event will affect
	* @param timeStampPriority: The timestamp at which the Event should be deployed
	* @param priority: A secondary priority 
	*/
	Event(GameObject *goRef, int timeStampPriority, int priority) {
		this->goRef = goRef;
		this->timeStampPriority = timeStampPriority;
		this->priority = priority;
	}

	/**
	* Constructor using the GameObject's ID in order to find and link the GameObject to this Event
	*/
	Event(int goID, GameObjectManager *goManager, int timeStampPriority, int priority) {
		this->goRef = goManager->find(goID);
		this->timeStampPriority = timeStampPriority;
		this->priority = priority;
	}

	/**
	* A function that executes when an Event is dispatched
	*/
	virtual void onEvent() = 0;

	/**
	* A less than operator that can be used to compare Events to each other
	* 
	* @param other: The other event for comparison
	* @returns Whether or not the Event is considered less than the other based on the two priority values
	*/
	bool operator<(Event other) const {
		return
			timeStampPriority < other.timeStampPriority || // Less than if timestamp is smaller than the other
			(timeStampPriority == other.timeStampPriority && priority < other.priority); // or if the priority value is lower if the timestamp is the same
	}
};

#endif
