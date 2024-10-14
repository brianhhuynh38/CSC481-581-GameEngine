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
		Utils::Vector2D m_velocity;
		// The acceleration of the object
		Utils::Vector2D m_acceleration;

		// Whether the collider is a trigger area (no collision) or not
		bool isTrigger;
		// The collider of the GameObject
		SDL_Rect collider;
		// Reference to the parent GameObject
		GameObject* m_parent;
		// Reference to the timeline (temp)
		Timeline* m_timeline;

		Physics* m_physics;

	public:
		void update() override {
			// Update physics if the object is not static
			if (!m_isStatic) {
				// Get deltaTime and convert into seconds
				float deltaTimeInSecs = m_timeline->getDeltaTime() / MICROSEC_PER_SEC;

				// Update velocity using acceleration
				m_velocity = m_velocity.add(m_acceleration.multConst(deltaTimeInSecs));

				// Update position using velocity
				m_parent->getComponent<Transform>()->updatePosition(m_velocity.multConst(deltaTimeInSecs));

				// TODO: Get gravity applying properly

				m_physics->applyGravity();

				//Utils::Vector2D gravityForce(0, m_gravity * entity->getMass());

				//// Gravity value is multiplied by 01 to make it a downward force
				////entity->updateVelocity(entity->getVelocity()->add(gravityForce.multConst(deltaTimeInSecs)));
				//entity->updateAcceleration(Utils::Vector2D(0, m_gravity * entity->getMass()).multConst(deltaTimeInSecs));
			}
		}
	};

}

#endif
