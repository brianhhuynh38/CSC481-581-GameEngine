#pragma once

#ifndef EVENT_H
#define EVENT_H

#include "GameObject.h"
#include "gameObjectManager.h"

#include "json.hpp"

using json = nlohmann::json;

namespace Events {

	#define EVENTS_SEND_ALL = 0

	/**
	* Class representation of the Event that houses a reference to a GameObject (if applicable) and 
	* multiple priority fields that help to compare between Events
	*/
	class Event {
	private:

		// Reference to the GameObject that this Event should be attached to
		std::vector<GameObject*> m_goRefVector;
		// A Timestamp value that is used to determine when the Event should be dispatched
		int64_t m_timeStampPriority;
		// The priority integer value used to help sort the priority queue alongside the Timestamp
		int m_priority;

	public:

		/**
		* Constructor for the Event struct
		*
		* @param goRef: A reference to the GameObject that the Event will affect
		* @param timeStampPriority: The timestamp at which the Event should be deployed
		* @param priority: A secondary priority
		*/
		Event(std::vector<GameObject*> goRef, int timeStampPriority, int priority);

		/**
		* Constructor using the GameObject's ID in order to find and link the GameObject to this Event
		*/
		Event(std::vector<int> goIDs, GameObjectManager* goManager, int timeStampPriority, int priority);

		/**
		* Constructor using Event flags to find and link the GameObjects to this Event
		*/
		Event(int flag, int timeStampPriority, int priority);

		/**
		* Destructor for an Event that gets rid of the reference to GameObject
		*/
		~Event();

		/**
		* A function that executes when an Event is dispatched
		*/
		virtual void onEvent() = 0;

		/**
		* Returns a string in JSON format for the Event, GameObjectUUID is sent as 0 (invalid) if there is no reference
		*/
		virtual void to_json(json &j);

		/**
		* Return the timestamp the event should execute at
		*/
		int64_t getTimeStampPriority();

		/**
		* Sets the time stamp to the specified parameter
		*/
		void setTimeStampPriority(int64_t timeStampPriority);

		/**
		* Returns the priority of the Event
		*/
		int getPriority();

		/**
		* Sets the priority of the Event
		*/
		void setPriority(int priority);

		/**
		* A less than operator that can be used to compare Events to each other
		*
		* @param other: The other event for comparison
		* @returns Whether or not the Event is considered less than the other based on the two priority values
		*/
		bool operator<(Event other) const {
			return
				m_timeStampPriority < other.m_timeStampPriority || // Less than if timestamp is smaller than the other
				(m_timeStampPriority == other.m_timeStampPriority && m_priority < other.m_priority); // or if the priority value is lower if the timestamp is the same
		}
	};
}

#endif
