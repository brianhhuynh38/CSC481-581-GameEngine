#pragma once

#ifndef EVENT_H
#define EVENT_H

#include "GameObject.h"

struct Event {
	// Reference to the GameObject that this Event should be attached to
	GameObject *goRef;
	//int m_goUUIDRef; // Only one of these should be used

	// A Timestamp value that is used to determine when the Event should be dispatched
	int timeStampPriority;
	// The priority integer value used to help sort the priority queue alongside the Timestamp
	int priority;

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
