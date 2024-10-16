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
		// The vector that defines the force with which the palyer jumps
		Utils::Vector2D jumpVector;
		// Struct that takes in player inputs
		InputHandler* m_inputHandler;
		// A reference to the GameObject parent that allows for inter-Object communication between components
		GameObject* m_parent;
	public:

		PlayerInputPlatformer();

		void update() override;
	};

}

#endif
