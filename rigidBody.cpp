#include "rigidBody.h"

#include "GameObject.h"
#include "physicsCalculator.h"
#include "collisions.h"
//#include "component.h"
//#include "vector2D.h"
//#include "physics.h"
//
//#include <SDL.h>

namespace Components {

	RigidBody::RigidBody(float mass, bool isKinematic, SDL_Rect collider, int colliderType, bool isTrigger, GameObject* parentRef) {
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

		m_isTrigger = isTrigger;

		// Set all references to necessary files
		m_parent = parentRef;
	}

	RigidBody::~RigidBody() {
		// Deletes velocity and acceleration pointers
		delete m_velocity;
		delete m_acceleration;
	}

	void RigidBody::update() {
		// Update physics if the object is not kinematic
		if (!m_isKinematic) {
			// Get deltaTime and convert into seconds
			float deltaTimeInSecs = m_parent->getDeltaTimeInSecsOfObject();

			// Update physics vectors
			PhysCalc::updatePhysicsVectors(deltaTimeInSecs, m_parent->getComponent<Transform>()->getPosition(), m_velocity, m_acceleration);

			// Apply gravity
			PhysCalc::applyGravity(deltaTimeInSecs, m_mass, m_acceleration);
		}
	}

	void RigidBody::updateCollisions(std::map<int, GameObject> goMap) {

	}

	/**
	 * Loop through entities, use their colliders to check the collisions, using checkCollision method
	 * This was referenced from the SDL2 tutorial:
	 * https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
	 */
	HitInfo checkCollisions(SDL_Rect *collider, std::map<int, GameObject> goMap) {
		// Create list iterator
		std::map<int, GameObject>::iterator iterGO;

		// Create hit responce for return
		HitInfo hInfo{ false };

		// Loop through entities, use their colliders to check the collisions, using checkCollision method
		for (iterGO = goMap.begin(); iterGO != goMap.end(); ++iterGO) { // Iterate through all entities

			// RigidBody of the other GameObject
			RigidBody *rb = iterGO->second.getComponent<RigidBody>();

			// Check if RigidBody is null
			if (rb) {
				// Get collider
				SDL_Rect *otherCollider = rb->getCollider();

				// Check if the collider does not belong to this RigidBody, then checks if there is an intersection between the two
				if (collider != otherCollider && SDL_HasIntersection(collider, otherCollider)) {

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
		*m_velocity = m_velocity->add(other);
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

}
