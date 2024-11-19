#include "playerInput.h"

#include "GameObject.h"
#include "physicsCalculator.h"
#include "inputEvent.h"
#include "global.h"

#include <SDL.h>

namespace Components {

	PlayerInputPlatformer::PlayerInputPlatformer() {
		m_maxMovementSpeed = 0;
		m_jumpVector = Utils::Vector2D(1, 1);
		m_parent = nullptr;
		m_canPressInput = true;
		m_isGrounded = false;
		m_inputHandler = nullptr;
		m_inputMemory = std::vector<uint8_t>();
		m_inputTimer = 0;
		m_nextInputOpen = true;
		//eventManager->registerEvent<Events::InputEvent>(m_parent);
	}

	PlayerInputPlatformer::PlayerInputPlatformer(float maxMovementSpeed, Utils::Vector2D jumpVector, GameObject* parentRef) {
		m_maxMovementSpeed = maxMovementSpeed;
		m_jumpVector = jumpVector;
		m_parent = parentRef;
		m_canPressInput = true;
		m_isGrounded = false;
		m_inputHandler = nullptr;
		m_inputMemory = std::vector<uint8_t>();
		m_inputTimer = 0;
		m_nextInputOpen = true;
		//eventManager->registerEvent<Events::InputEvent>(m_parent);
	}

	void PlayerInputPlatformer::update() {
		if (m_canPressInput && m_inputHandler) {
			// Get necessary components needed to determine movement type
			RigidBody* rb = m_parent->getComponent<RigidBody>();
			// Vector defining direction the player will move in
			Utils::Vector2D moveVector = Utils::Vector2D(0, 0);

			uint8_t moveByte = m_inputHandler->inputByte;

			// Movement check
			if ((moveByte & INPUT_RIGHT) == INPUT_RIGHT) {
				moveVector.x += m_maxMovementSpeed;
			}
			if ((moveByte & INPUT_LEFT) == INPUT_LEFT) {
				moveVector.x -= m_maxMovementSpeed;
			}
			if ((moveByte & INPUT_DOWN) == INPUT_DOWN) {
				// Nothing currently
			}
			if ((moveByte & INPUT_UP) == INPUT_UP && m_isGrounded) {
				// Jumping check
				moveVector.y = m_jumpVector.y;
				m_isGrounded = false;
			}
			else {
				// gravity
				moveVector.y += rb->getAcceleration()->y;
			}

			// Determines whether to record input into the input memory
			if (m_nextInputOpen && moveByte != INPUT_NONE) {
				// If input open, register the latest input into the memory and update timer
				m_inputMemory.push_back(moveByte);
				m_inputTimer = 0;
				m_nextInputOpen = false;
			}
			else {
				if (moveByte == INPUT_NONE) {
					m_nextInputOpen = true;
					m_inputTimer = 0;
				}
			}

			// Once inputs have not been registered in 6 iterations, clear the input memory
			if (m_inputTimer >= 6) {
				m_inputMemory.clear();
				m_inputTimer = 0;
				m_nextInputOpen = true;
			}

			// Compare current input memory to any special inputs (I wanted to represent this with a tree, but I don't have time to implement one)
			// Max depth is 3
			int verificationCounterForTeleport = 0;
			for (uint8_t input : m_inputMemory) {
				// Check for correct inputs in sequence
				if (input == INPUT_TELEPORT[verificationCounterForTeleport]) {
					verificationCounterForTeleport++;
				}
				else { // Reset if no longer in sequence
					verificationCounterForTeleport = 0;
				}
				// If sequence matched, break
				if (verificationCounterForTeleport == 3) {
					break;
				}
			}
			
			// Create GameObject vector
			std::vector<GameObject*> goVec = std::vector<GameObject*>();
			goVec.push_back(m_parent);

			// Raise an InputEvent to handle player movement

			if (verificationCounterForTeleport == 3) {
				eventManager->raiseEvent(new Events::InputEvent(goVec, m_parent->getCurrentTimeStamp(), 0, *m_inputHandler, moveVector, IFLAG_TELEPORT));
				m_inputMemory.clear();
			}
			else {
				eventManager->raiseEvent(new Events::InputEvent(goVec, m_parent->getCurrentTimeStamp(), 0, *m_inputHandler, moveVector));
				// Increment inputTimer
				m_inputTimer++;
			}
		}
	}

	float PlayerInputPlatformer::getMaxSpeed() {
		return m_maxMovementSpeed;
	}

	Utils::Vector2D PlayerInputPlatformer::getJumpVector() {
		return m_jumpVector;
	}

	InputHandler* PlayerInputPlatformer::getInputHandler() {
		return m_inputHandler;
	}

    bool PlayerInputPlatformer::isGrounded() {
		return m_isGrounded;
	}

    void PlayerInputPlatformer::setIsGrounded(bool isGrounded) {
		m_isGrounded = isGrounded;
	}

	void PlayerInputPlatformer::setInputHandler(InputHandler* inputHandler) {
		m_inputHandler = inputHandler;
	}


}
