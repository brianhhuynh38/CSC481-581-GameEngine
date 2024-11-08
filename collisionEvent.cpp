#include "collisionEvent.h"

#include "deathEvent.h"

#include "global.h"
#include "transform.h"
#include "rigidBody.h"
#include "boundaryZone.h"

namespace Events {

	CollisionEvent::CollisionEvent(std::vector<GameObject*> goRefVector, int64_t timeStampPriority, int priority, HitInfo* hitInfo) {
		this->m_goRefVector = goRefVector;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
		this->m_hitInfo = hitInfo;
	}

	void CollisionEvent::onEvent() const {
		for (GameObject* go : m_goRefVector) {
			switch (m_hitInfo->colliderType) {
				case 0: // Standard collision
				{
					// Get Transform component of the GameObject to manipulate position
					Components::Transform* transform = go->getComponent<Components::Transform>();
					Components::RigidBody* rb = go->getComponent<Components::RigidBody>();

					// Sets the amount of distance and velocity changed during the collision
					m_hitInfo->posMover = m_hitInfo->hitVector.normalizeVector().multConst(rb->getVelocity()->getMagnitude() * go->getDeltaTimeInSecsOfObject() * -1);
					m_hitInfo->velMover = Utils::Vector2D(0, rb->getVelocity()->y * -1);

					// Updates the position and velocity of the object (TODO: Not sure if this would work)
					transform->updatePosition(m_hitInfo->posMover);
					rb->updateVelocity(m_hitInfo->velMover);

					// Set position of the collider to the position of the transform
					rb->setColliderCoordinates(transform->getPosition()->x, transform->getPosition()->y);
					break;
				}
				case 1: // Death Boundary
				{
					// Call death event
					std::vector<GameObject*> goVec = std::vector<GameObject*>();
					goVec.push_back(go);
					Events::DeathEvent* de = new Events::DeathEvent(goVec, go->getCurrentTimeStamp(), 1);
					eventManager->raiseEvent(de);
					break;
				}
				case 2: // Camera Change Collision
				{
					BoundaryZone* boundaryZone = static_cast<BoundaryZone*>(m_hitInfo->collidedObj);

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
					break;
				}
				default:
				{
					std::cerr << "Invalid collider type: " << m_hitInfo->colliderType << "\n";
					break;
				}
			}
		}
	}

}
