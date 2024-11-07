#include "deathEvent.h"

#include "eventManager.h"
#include "spawnEvent.h"
#include "global.h"

namespace Events {

	DeathEvent::DeathEvent(std::vector<GameObject*> goRefVector, int64_t timeStampPriority, int priority) {
		this->m_goRefVector = goRefVector;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	void DeathEvent::onEvent() const {
		Events::SpawnEvent se = Events::SpawnEvent(m_goRefVector, m_timeStampPriority, m_priority);
		eventManager->raiseEvent(se);
	}

}