#pragma once

#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H

#include "component.h"
#include "GameObject.h"

#include "input.h"
#include "vector2D.h"

namespace Components {

	class PlayerInputPlatformer : public virtual Component {
	protected:
		// The maximum speed the player moves at
		float maxMovementSpeed;
		// The vector that provides the player
		Utils::Vector2D jumpVector;
		InputHandler* m_inputHandler;
		GameObject* m_parent;
	public:

		PlayerInputPlatformer();

		void update() override;
	};

}

#endif
