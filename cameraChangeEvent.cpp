#include "cameraChangeEvent.h"

#include "vector2D.h"
#include "transform.h"
#include "playerGO.h"
#include "boundaryZone.h"

namespace Events {

	CameraChangeEvent::CameraChangeEvent(std::vector<GameObject*> goRefVector, int64_t timeStampPriority, int priority) {
		this->m_goRefVector = goRefVector;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	void CameraChangeEvent::onEvent() {
		// Spawn each Player game object at its specified spawn point
		for (GameObject* go : m_goRefVector) {
			std::lock_guard<std::mutex> lock(go->mutex);
			BoundaryZone* boundaryZone = static_cast<BoundaryZone*>(go);
			// Check switch (which side collided from)
			if (boundaryZone->getCurrentPos().equals(boundaryZone->getPos1()) && boundaryZone->checkCooldown()) {
				// Change camera location to the other one
				boundaryZone->setCurrentPos(boundaryZone->getPos2());
				boundaryZone->initiateTimer(15);
			}
			else if (boundaryZone->getCurrentPos().equals(boundaryZone->getPos2()) && boundaryZone->checkCooldown()) {
				// Change camera location to the other one
				boundaryZone->setCurrentPos(boundaryZone->getPos1());
				boundaryZone->initiateTimer(15);
			}
		}
	}

}
