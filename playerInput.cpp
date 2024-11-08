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
		//eventManager->registerEvent<Events::InputEvent>(m_parent);
	}

	PlayerInputPlatformer::PlayerInputPlatformer(float maxMovementSpeed, Utils::Vector2D jumpVector, GameObject* parentRef) {
		m_maxMovementSpeed = maxMovementSpeed;
		m_jumpVector = jumpVector;
		m_parent = parentRef;
		m_canPressInput = true;
		m_isGrounded = false;
		m_inputHandler = nullptr;
		//eventManager->registerEvent<Events::InputEvent>(m_parent);
	}

	void PlayerInputPlatformer::update() {
		if (m_canPressInput && m_inputHandler) {

			Transform* transform = m_parent->getComponent<Transform>();
			RigidBody* rb = m_parent->getComponent<RigidBody>();

			bool isPhysicsBased = rb && !rb->isKinematic();

			Utils::Vector2D moveVector = Utils::Vector2D(0, 0);

			// horizontal movement
			if ((m_inputHandler->inputByte & INPUT_RIGHT) == INPUT_RIGHT) {
				if (isPhysicsBased) {
					moveVector.x += m_maxMovementSpeed;
				}
				else {
					moveVector.x = m_maxMovementSpeed;
				}
			}
			if ((m_inputHandler->inputByte & INPUT_LEFT) == INPUT_LEFT) {
				if (isPhysicsBased) {
					moveVector.x -= m_maxMovementSpeed;
				}
				else {
					moveVector.x = -m_maxMovementSpeed;
				}
			}

			if ((m_inputHandler->inputByte & INPUT_DOWN) == INPUT_DOWN) {
				// nothing
			}

			// Jump
			if ((m_inputHandler->inputByte & INPUT_UP) == INPUT_UP && m_isGrounded) {
				moveVector.y = m_jumpVector.y;
				m_isGrounded = false;
			}

			else {
				// gravity
				moveVector.y += rb->getAcceleration()->y;
			}

			// Create GameObject vector
			std::vector<GameObject*> goVec = std::vector<GameObject*>();
			goVec.push_back(m_parent);
			// Raise an InputEvent to handle player movement
			eventManager->raiseEvent(new Events::InputEvent(goVec, m_parent->getCurrentTimeStamp(), 0, *m_inputHandler, moveVector));			
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
