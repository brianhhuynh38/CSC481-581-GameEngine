#include "event.h"

namespace Events {

	/**
	* Constructor for the Event struct
	*
	* @param goRef: A reference to the Gameobject that the Event will affect
	* @param timeStampPriority: The timestamp at which the Event should be deployed
	* @param priority: A secondary priority
	*/
	Event::Event(std::vector<GameObject*> goRefVector, int timeStampPriority, int priority) {
		this->m_goRefVector = goRefVector;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	/**
	* Destructor for an Event that gets rid of the reference to GameObject
	*/
	Event::~Event() {
		this->m_goRefVector.clear();
	}

	/**
	* Returns a string in JSON format for the Event, GameObjectUUID is sent as 0 (invalid) if there is no reference
	*/
	void Event::to_json(json& j) {
		// Add all fields to the json, but records gameObject ID as 0 if there is no GameObject reference
		std::vector<int> idVector = std::vector<int>();
		for (GameObject *goRef : m_goRefVector) {
			idVector.push_back(goRef->getUUID());
		}
		j["goUUIDs"] = idVector;
		j["timeStampPriority"] = m_timeStampPriority;
		j["priority"] = m_priority;
	}

	/**
	* Return the timestamp the event should execute at
	*/
	int64_t Event::getTimeStampPriority() const {
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
