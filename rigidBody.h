#pragma once

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "GameObject.h"
#include "component.h"
#include "vector2D.h"
#include "physics.h"

#include <SDL.h>

namespace Components {

	class RigidBody : public virtual Component {
	protected:
		// Whether the object moves and is affected by physics
		bool m_isKinematic;
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
		// Reference to the parent GameObject to communicate with other components
		GameObject *m_parent;
		// Reference to physics object
		//Physics *m_physics;

	public:

		RigidBody(float mass, bool isKinematic, SDL_Rect collider, bool isTrigger, GameObject* parentRef, Physics* physicsRef);

		~RigidBody() override;

		void update() override;

		/**
		* Returns the mass of the GameObject
		*/
		float getMass();

		/**
		* Sets the mas of the GameObject
		*/
		void setMass(float mass);

		/**
		* Returns a pointer to the Object's velocity
		*/
		Utils::Vector2D* getVelocity();

		/**
		* Adds the given vector to the current velocity
		*/
		void updateVelocity(Utils::Vector2D other);

		/**
		* Sets the velocity vector to the given parameter vector
		*/
		void setVelocity(Utils::Vector2D other);

		/**
		* Returns a pointer to the acceleration of the GameObject
		*/
		Utils::Vector2D* getAcceleration();

		/**
		* Updates the acceleration by adding the other parameter to the current value
		*/
		void updateAcceleration(Utils::Vector2D other);

		/**
		* Sets the acceleration value to the provided vector
		*/
		void setAcceleration(Utils::Vector2D other);
	};

}

#endif
