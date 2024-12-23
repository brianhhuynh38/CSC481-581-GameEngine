#include "rigidBody.h"

#include "GameObject.h"
#include "physicsCalculator.h"

#include <algorithm>
#include "playerGO.h"
#include "boundaryZone.h"
#include "collisionEvent.h"
#include "physicsEvent.h"
#include "global.h"


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
				// If it hits something, raise a collision event
				std::vector<GameObject*> goVec = std::vector<GameObject*>();
				goVec.push_back(m_parent);
				eventManager->raiseEvent(new Events::CollisionEvent(goVec, m_parent->getCurrentTimeStamp(), 0, &m_mostRecentCollisionInfo));
			}
		}
	}

	void RigidBody::update() {
		// Raise PhysicsEvent in order to update physics vectors for this GameObject
		std::vector<GameObject*> goVec = std::vector<GameObject*>();
		goVec.push_back(m_parent);
		eventManager->raiseEvent(new Events::PhysicsEvent(goVec, m_parent->getCurrentTimeStamp(), 0));
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
		vel.y = std::max(std::min(vel.y, 100.0f), -300.0f);
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
		acc.y = std::max(std::min(acc.y, 15.0f), -65.0f);
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
