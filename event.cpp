#include "event.h"

namespace Events {

	/**
	* Constructor for the Event struct
	*
	* @param goRef: A reference to the Gameobject that the Event will affect
	* @param timeStampPriority: The timestamp at which the Event should be deployed
	* @param priority: A secondary priority
	*/
	Event::Event(GameObject* goRef, int timeStampPriority, int priority) {
		this->m_goRef = goRef;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	/**
	* Constructor using the GameObject's ID in order to find and link the GameObject to this Event
	*/
	Event::Event(int goID, GameObjectManager* goManager, int timeStampPriority, int priority) {
		this->m_goRef = goManager->find(goID);
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	/**
	* Destructor for an Event that gets rid of the reference to GameObject
	*/
	Event::~Event() {
		this->m_goRef = nullptr;
	}

	/**
	* Returns a string in JSON format for the Event, GameObjectUUID is sent as 0 (invalid) if there is no reference
	*/
	std::string Event::serialize() {
		json j;
		// Add all fields to the json, but records gameObject ID as 0 if there is no GameObject reference
		j["goUUID"] = m_goRef ? m_goRef->getUUID() : 0;
		j["timeStampPriority"] = m_timeStampPriority;
		j["priority"] = m_priority;
		// Return json string
		return j.dump();
	}

	/**
	* Return the timestamp the event should execute at
	*/
	int64_t Event::getTimeStampPriority() {
		return m_timeStampPriority;
	}

	/**
	* Sets the time stamp to the specified parameter
	*/
	void Event::setTimeStampPriority(int64_t timeStampPriority) {
		m_timeStampPriority = timeStampPriority;
	}

	/**
	* Returns the priority of the Event
	*/
	int Event::getPriority() {
		return m_priority;
	}

	/**
	* Sets the priority of the Event
	*/
	void Event::setPriority(int priority) {
		m_priority = priority;
	}
}