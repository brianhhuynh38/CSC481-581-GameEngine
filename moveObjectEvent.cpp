#include "moveObjectEvent.h"


namespace Events {

	/**
	* Constructor for MoveObjectEvent
	*/
	MoveObjectEvent::MoveObjectEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, zmq::socket_t* socket) {
		this->m_goRefVector = goRef;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	void MoveObjectEvent::onEvent() const {
		for (GameObject* go : m_goRefVector) {
			//go->update();
		}
	}

}