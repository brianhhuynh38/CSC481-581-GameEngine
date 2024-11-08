#include "rigidBody.h"

#include "GameObject.h"
#include "physicsCalculator.h"

#include <algorithm>
#include "playerGO.h"
#include "boundaryZone.h"
#include "deathEvent.h"
#include "global.h"
//#include "transform.h"
//#include "component.h"
//#include "vector2D.h"
//#include "physics.h"
//
//#include <SDL.h>

namespace Components {

	RigidBody::RigidBody(float mass, bool isKinematic, SDL_Rect collider, int colliderType, GameObject* parentRef) {
		// Create new vectors at (0,0) for velocity and acceleration
		m_velocity = new Utils::Vector2D();
		m_acceleration = new Utils::Vector2D();
		// Set mass and whether the object is able to move/be able to be affected by physics
		m_mass = mass;
		m_isKinematic = isKinematic;
		// Set collider options and whether it serves as a trigger
		m_collider = new SDL_Rect();
		*m_collider = collider;
		m_colliderType = colliderType;
		// Set all references to necessary files
		m_parent = parentRef;
	}

	RigidBody::~RigidBody() {
		// Deletes velocity and acceleration pointers
		delete m_velocity;
		delete m_acceleration;
		delete m_collider;
	}

	void RigidBody::updateCollisions(std::map<int, GameObject*>& goMap) {
		// Only update collisions if the object is not kinematic to save some processing time
		if (!m_isKinematic) {

			// Get hit information from checkCollisions
			m_mostRecentCollisionInfo = checkObjectCollisions(m_collider, goMap);
			// Check if the object collided with anything
			if (m_mostRecentCollisionInfo.hit) {
				if (m_mostRecentCollisionInfo.colliderType == 0) { // Standard collision
					// Get Transform component of the GameObject to manipulate position
					Transform* transform = m_parent->getComponent<Transform>();

					//std::cout << "HitInfo: " << m_mostRecentCollisionInfo.hitVector.toString();

					// Sets the amount of distance and velocity changed during the collision
					m_mostRecentCollisionInfo.posMover = m_mostRecentCollisionInfo.hitVector.normalizeVector().multConst(m_velocity->getMagnitude() * m_parent->getDeltaTimeInSecsOfObject() * -1);
					m_mostRecentCollisionInfo.velMover = Utils::Vector2D(0, m_velocity->y * -1);

					// Updates the position and velocity of the object (TODO: Not sure if this would work)
					transform->updatePosition(m_mostRecentCollisionInfo.posMover);
					updateVelocity(m_mostRecentCollisionInfo.velMover);

					// Set position of the collider to the position of the transform
					m_collider->x = transform->getPosition()->x;
					m_collider->y = transform->getPosition()->y;
				}
				else if (m_mostRecentCollisionInfo.colliderType == 1) { // Death zone collision
					/*Transform* transform = m_parent->getComponent<Transform>();
					auto* player = static_cast<PlayerGO*>(m_parent);
					Transform* spawnTransform = player->getSpawn()->getComponent<Transform>();
					transform->setPosition(spawnTransform->getPosition()->x, spawnTransform->getPosition()->y);*/

					// Call spawn event
					std::vector<GameObject*> go = std::vector<GameObject*>();
					go.push_back(m_parent);
					Events::DeathEvent* de = new Events::DeathEvent(go, m_parent->getCurrentTimeStamp(), 1);
					eventManager->raiseEvent(de);
					//m_parent->getEventManager()->raiseEvent(se);
					//EventManager::raiseEvent(se);
				}
				else if (m_mostRecentCollisionInfo.colliderType == 2) { // Boundary collision
					auto* boundaryZone = static_cast<BoundaryZone*>(m_mostRecentCollisionInfo.collidedObj);

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
	}

	void RigidBody::update() {
		// Update physics if the object is not kinematic
		if (!m_isKinematic) {
			// Get deltaTime and convert into seconds
			float deltaTimeInSecs = m_parent->getDeltaTimeInSecsOfObject();

			// Get the parent GameObject's transform component
			Utils::Vector2D *position = m_parent->getComponent<Transform>()->getPosition();

			// Update physics vectors
			PhysCalc::updatePhysicsVectors(deltaTimeInSecs, position, m_velocity, m_acceleration);

			// Set position of the collider to the position of the transform
			m_collider->x = position->x;
			m_collider->y = position->y;

			// Apply gravity
			PhysCalc::applyGravity(deltaTimeInSecs, m_mass, m_acceleration);

		}
	}


	/**
	 * Loop through entities, use their colliders to check the collisions, using checkCollision method
	 * This was referenced from the SDL2 tutorial:
	 * https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
	 */
	HitInfo RigidBody::checkObjectCollisions(SDL_Rect *collider, std::map<int, GameObject*>& goMap) {

		// Create list iterator
		std::map<int, GameObject*>::iterator iterGO;

		// Create hit response for return
		HitInfo hInfo{ false };

		// Loop through entities, use their colliders to check the collisions, using checkCollision method
		for (iterGO = goMap.begin(); iterGO != goMap.end(); ++iterGO) { // Iterate through all entities

			// RigidBody of the other GameObject
			RigidBody *rb = iterGO->second->getComponent<RigidBody>();

			// Check if RigidBody is null
			if (rb) {
				// Get collider
				SDL_Rect *otherCollider = rb->getCollider();

				// Check if the collider does not belong to this RigidBody, then checks if there is an intersection between the two
				if (collider != otherCollider && SDL_HasIntersection(collider, otherCollider)) {

					//std::cout << "SDL_HasIntersection returned true\n";

					// Calculate penetration depth
					int overlapX = std::min(collider->x + collider->w, otherCollider->x + otherCollider->w) -
						std::max(collider->x, otherCollider->x);
					int overlapY = std::min(collider->y + collider->h, otherCollider->y + otherCollider->h) -
						std::max(collider->y, otherCollider->y);

					// Determine the penetration depth
					Utils::Vector2D penetrationDepth;

					if (overlapX < overlapY) {
						penetrationDepth.x = overlapX;
						penetrationDepth.y = 0; // No vertical penetration
					}
					else {
						penetrationDepth.x = 0; // No horizontal penetration
						penetrationDepth.y = overlapY;
					}

					Utils::Vector2D direction = Utils::Vector2D(otherCollider->x, otherCollider->y).add(Utils::Vector2D(-collider->x, -collider->y));

					hInfo.hit = true;
					hInfo.hitVector = direction;
					hInfo.collisionRect = *otherCollider;
					hInfo.penetrationDepth = penetrationDepth;
					hInfo.colliderType = rb->getColliderType();
					hInfo.collidedObj = iterGO->second;

					//std::cout << "HitInfo hit: " << hInfo.hit << "\nHitInfo direction: " << hInfo.hitVector.toString() <<
					//	"\nHitInfo penetration depth: " << penetrationDepth.toString() << "\n";
				}
			}
		}

		return hInfo;
	}

	/**
	* Returns the mass of the GameObject
	*/
	float RigidBody::getMass() {
		return m_mass;
	}

	/**
	* Sets the mas of the GameObject
	*/
	void RigidBody::setMass(float mass) {
		m_mass = mass;
	}

	/**
	* Returns a pointer to the Object's velocity
	*/
	Utils::Vector2D* RigidBody::getVelocity() {
		return m_velocity;
	}

	/**
	* Adds the given vector to the current velocity
	*/
	void RigidBody::updateVelocity(Utils::Vector2D other) {
		Utils::Vector2D vel = (m_velocity->add(other));
		vel.x = std::max(std::min(vel.x, 100.0f), -100.0f);
		vel.y = std::max(std::min(vel.y, 100.0f), -100.0f);
		*m_velocity = vel;
	}

	/**
	* Sets the velocity vector to the given parameter vector
	*/
	void RigidBody::setVelocity(Utils::Vector2D other) {
		*m_velocity = other;
	}

	/**
	* Returns a pointer to the acceleration of the GameObject
	*/
	Utils::Vector2D* RigidBody::getAcceleration() {
		return m_acceleration;
	}

	/**
	* Updates the acceleration by adding the other parameter to the current value
	*/
	void RigidBody::updateAcceleration(Utils::Vector2D other) {
		Utils::Vector2D acc = (m_velocity->add(other));
		
		acc.x = std::max(std::min(acc.x, 15.0f), -15.0f);
		acc.y = std::max(std::min(acc.y, 15.0f), -15.0f);
		*m_acceleration = m_acceleration->add(other);
	}

	/**
	* Sets the acceleration value to the provided vector
	*/
	void RigidBody::setAcceleration(Utils::Vector2D other) {
		*m_acceleration = other;
	}

	SDL_Rect* RigidBody::getCollider() {
		return m_collider;
	}

	void RigidBody::setColliderCoordinates(float x, float y) {
		m_collider->x = x;
		m_collider->y = y;
	}

	void RigidBody::setColliderCoordinates(Utils::Vector2D newCoords) {
		m_collider->x = newCoords.x;
		m_collider->y = newCoords.y;
	}

	bool RigidBody::isKinematic() {
		return m_isKinematic;
	}

	void RigidBody::setIsKinematic(bool isKinematic) {
		m_isKinematic = isKinematic;
	}

	int RigidBody::getColliderType() {
		return m_colliderType;
	}

	HitInfo RigidBody::getMostRecentCollisionInfo() {
		return m_mostRecentCollisionInfo;
	}
}
