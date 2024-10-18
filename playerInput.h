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
		// Whether the player can input
		bool m_canPressInput;
		// Whether the player is on the ground
		bool m_isGrounded;
		// The maximum speed the player moves at
		float m_maxMovementSpeed;
		// The vector that defines the force with which the palyer jumps
		Utils::Vector2D m_jumpVector;
		// Struct that takes in player inputs
		InputHandler* m_inputHandler;
		// A reference to the GameObject parent that allows for inter-Object communication between components
		GameObject* m_parent;

		// Helper method to move the player transform
		void movePlayer(Utils::Vector2D movementVector, int axis);
	public:
		/* Default constructor */
		PlayerInputPlatformer();

		/* Constructor */
		PlayerInputPlatformer(float maxMovementSpeed, Utils::Vector2D jumpVector, InputHandler* inputHandler, GameObject* parentRef);

		void update() override;
	};

}

#endif
