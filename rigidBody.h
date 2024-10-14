#pragma once

#include "component.h"
#include "vector2D.h"
#include "timeline.h"
#include "definitions.h"
#include "physics.h"

#include <SDL.h>

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

namespace Component {

	class RigidBody : public virtual Component {
	private:
		// Whether the object moves and is affected by physics
		bool m_isStatic;
		// The mass of the object
		float m_mass;

		// The velocity of the object
		Utils::Vector2D *m_velocity;
		// The acceleration of the object
		Utils::Vector2D *m_acceleration;

		// Whether the collider is a trigger area (no collision) or not
		bool m_isTrigger;
		// The collider of the GameObject
		SDL_Rect m_collider;
		// Reference to the parent GameObject
		GameObject* m_parent;
		// Reference to the timeline (temp)
		Timeline* m_timeline;
		// Reference to physics object
		Physics* m_physics;

	public:

		RigidBody(float mass, bool isStatic, SDL_Rect collider, bool isTrigger, GameObject *parentRef, Timeline *timelineRef, Physics *physicsRef) {
			// Create new vectors at (0,0) for velocity and acceleration
			m_velocity = new Utils::Vector2D();
			m_acceleration = new Utils::Vector2D();
			// Set mass and whether the object is able to move/be able to be affected by physics
			m_mass = mass;
			m_isStatic = isStatic;
			// Set collider options and whether it serves as a trigger
			m_collider = collider;
			m_isTrigger = isTrigger;

			// Set all references to necessary files
			m_parent = parentRef;
			m_timeline = timelineRef;
			m_physics = physicsRef;
		}

		~RigidBody() override {
			// Deletes velocity and acceleration pointers
			delete m_velocity;
			delete m_acceleration;
		}

		void update() override {
			// Update physics if the object is not static
			if (!m_isStatic) {
				// Get deltaTime and convert into seconds
				float deltaTimeInSecs = m_timeline->getDeltaTime() / MICROSEC_PER_SEC;

				// Update physics vectors
				m_physics->updatePhysicsVectors(deltaTimeInSecs, m_parent->getComponent<Transform>()->getPosition(), m_velocity, m_acceleration);

				// Apply gravity
				m_physics->applyGravity(deltaTimeInSecs, m_mass, m_acceleration);
			}
		}

		/**
		* Returns the mass of the GameObject
		*/
		float getMass() {
			return m_mass;
		}

		/**
		* Sets the mas of the GameObject
		*/
		void setMass(float mass) {
			m_mass = mass;
		}

		/**
		* Returns a pointer to the Object's velocity
		*/
		Utils::Vector2D* getVelocity() {
			return m_velocity;
		}

		/**
		* Adds the given vector to the current velocity
		*/
		void updateVelocity(Utils::Vector2D other) {
			*m_velocity = m_velocity->add(other);
		}

		/**
		* Sets the velocity vector to the given parameter vector
		*/
		void setVelocity(Utils::Vector2D other) {
			*m_velocity = other;
		}

		/**
		* Returns a pointer to the acceleration of the GameObject
		*/
		Utils::Vector2D* getAcceleration() {
			return m_acceleration;
		}

		/**
		* Updates the acceleration by adding the other parameter to the current value
		*/
		void updateAcceleration(Utils::Vector2D other) {
			*m_acceleration = m_acceleration->add(other);
		}

		/**
		* Sets the acceleration value to the provided vector
		*/
		void setAcceleration(Utils::Vector2D other) {
			*m_acceleration = other;
		}
	};

}

#endif
