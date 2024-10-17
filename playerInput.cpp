#include "playerInput.h"

#include "component.h"
#include "input.h"
#include "vector2D.h"
#include "physicsCalculator.h"

#include <SDL.h>

namespace Components {

	PlayerInputPlatformer::PlayerInputPlatformer() {
		m_maxMovementSpeed = 0;
		m_jumpVector = Utils::Vector2D(1, 1);
		m_inputHandler = nullptr;
		m_parent = nullptr;
	}

	PlayerInputPlatformer::PlayerInputPlatformer(float maxMovementSpeed, Utils::Vector2D jumpVector, InputHandler* inputHandler, GameObject* parentRef) {
		m_maxMovementSpeed = maxMovementSpeed;
		m_jumpVector = jumpVector;
		m_inputHandler = inputHandler;
		m_parent = parentRef;
	}

	// Helper method?


	void PlayerInputPlatformer::update() {
		// TODO: Update physics vectors and movement
	}


}
