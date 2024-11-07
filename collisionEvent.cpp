#include "collisionEvent.h"


namespace Events {

	CollisionEvent::CollisionEvent(std::vector<GameObject*> goRefVector, int64_t timeStampPriority, int priority, GameObject* otherGORef) {
		this->m_goRefVector = goRefVector;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
		this->m_otherGORef = otherGORef;
	}

	void CollisionEvent::onEvent() const {

	}

}
