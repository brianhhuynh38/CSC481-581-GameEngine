#pragma once

#include "component.h"
#include "vector2D.h"
#include "timeline.h"

#include <SDL.h>

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

namespace Component {

	class RigidBody : public virtual Component {
	private:
		bool m_isStatic;
		float m_mass;

		Utils::Vector2D m_velocity;
		Utils::Vector2D m_acceleration;

		bool isTrigger;
		SDL_Rect collider;
		GameObject *m_parent;
	public:
		void update() override {
			// TODO: Update physics vectors
		}
	};

}

#endif
