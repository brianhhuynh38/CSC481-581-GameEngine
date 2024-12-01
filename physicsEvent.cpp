#include "physicsEvent.h"

#include "rigidBody.h"
#include "transform.h"
#include "physicsCalculator.h"

namespace Events {

	PhysicsEvent::PhysicsEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority) {
		m_goRefVector = goRef;
		m_timeStampPriority = timeStampPriority;
		m_priority = priority;
	}
	
	void PhysicsEvent::onEvent() {
		for (GameObject* go : m_goRefVector) {
			Components::RigidBody* rb = go->getComponent<Components::RigidBody>();
			// Update physics if the object is not kinematic
			if (!rb->isKinematic()) {
				// Get deltaTime and convert into seconds
				float deltaTimeInSecs = go->getDeltaTimeInSecsOfObject();

				// Get the parent GameObject's transform component
				Utils::Vector2D* position = go->getComponent<Components::Transform>()->getPosition();

				// Update physics vectors
				PhysCalc::updatePhysicsVectors(deltaTimeInSecs, position, rb->getVelocity(), rb->getAcceleration());

				// Set position of the collider to the position of the transform
				rb->setColliderCoordinates(position->x, position->y);

				// Apply gravity
				PhysCalc::applyGravity(deltaTimeInSecs, rb->getMass(), rb->getAcceleration());

			}
		}
	}

}
