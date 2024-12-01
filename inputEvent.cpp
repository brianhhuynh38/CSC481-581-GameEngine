#include "inputEvent.h"

#include "startPlaybackEvent.h"
#include "startRecordingEvent.h"
#include "stopRecordingEvent.h"

#include "vector2D.h"
#include "rigidBody.h"
#include "playerInput.h"
#include "global.h"

namespace Events {

	InputEvent::InputEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, InputHandler inputHandler, Utils::Vector2D movementVector, IFlag inputFlag) {
		this->m_goRefVector = goRef;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
		this->m_inputHandler = inputHandler;
		this->m_movementVector = movementVector;
		this->m_flag = inputFlag;
	}

	void InputEvent::onEvent() {
		for (GameObject* go : m_goRefVector) {
			std::lock_guard<std::mutex> lock(go->mutex);

			// Applies any special inputs that are defined below
			applySpecialInput(go, m_flag);
			applyRecorderInput();

			float deltaTimeInSecs = go->getDeltaTimeInSecsOfObject();
			Utils::Vector2D posMover = Utils::Vector2D(0, 0); // amount to change positon by
			Utils::Vector2D velMover = Utils::Vector2D(0, 0); // amount to change velocity by
			float decelerationRate = 10;

			// Initialize moveVector, which is used to update player movement
			Utils::Vector2D moveVector = Utils::Vector2D(m_movementVector.x * deltaTimeInSecs, 0);

			Components::Transform* transform = go->getComponent<Components::Transform>();
			Components::RigidBody* rb = go->getComponent<Components::RigidBody>();

			// Get reference to RigidBody velocity
			Utils::Vector2D* velocity = rb->getVelocity();

			// moving on x-axis
			if (m_movementVector.x != 0.0f) {
				// check if changing directions
				if ((m_movementVector.x > 0 && velocity->x < 0) || (m_movementVector.x < 0 && velocity->x > 0)) {
					// increase rate of accelaration until moving in desired direction
					velMover.x = moveVector.x * decelerationRate;
				}
				else {
					velMover = moveVector;
				}
			}
			else {
				// moves velocity towards zero.
				velMover.x = -decelerationRate * velocity->x * deltaTimeInSecs;
				if (rb->getVelocity()->x > 2 || rb->getVelocity()->x < -2) {
					velMover.x = -decelerationRate * velocity->x * deltaTimeInSecs;
				}
				else {
					velMover.x = -velocity->x;
				}
			}

			posMover.x = velocity->x * deltaTimeInSecs;

			// moving on y-axis
			if (m_movementVector.y != 0.0f) {
				moveVector.y = m_movementVector.y * deltaTimeInSecs;
				velMover.y = moveVector.y;

				posMover.y = velocity->y * deltaTimeInSecs;
			}
			else {
				// moves velocity towards zero.
				// if the absolute value of the player's velocity is less than 2, it will then cancel out the velocity with no decceleration.
				if (velocity->y > 2) {
					velMover.y = -5 * velocity->y * deltaTimeInSecs;
				}
				else if (velocity->y < -2) {
					velMover.y = 5 * velocity->y * deltaTimeInSecs;
				}
				else {
					velMover.y = -velocity->y;
				}
			}

			// Move player
			rb->updateVelocity(velMover);
			transform->updatePosition(posMover);

			// Move colliders to player position
			rb->setColliderCoordinates(*transform->getPosition());

			// Gets the most recent collision information frfom RigidBody
			HitInfo hInfo = rb->getMostRecentCollisionInfo();

			// Determine grounded status
			Components::PlayerInputPlatformer* pi = go->getComponent<Components::PlayerInputPlatformer>();
			// If the object collided with something
			if (hInfo.hit && m_movementVector.y != 0) { // y-axis collision
				// set as grounded if player was moving down during y-axis collision
				pi->setIsGrounded(velocity->y >= 0);
			}
			// if there is no collision on the y-axis, set isGrounded to false
			else if (m_movementVector.y != 0) {
				pi->setIsGrounded(false);
			}
		}
	}

	void InputEvent::applySpecialInput(GameObject* go, IFlag flag) {
		switch (flag) {
			case IFLAG_TELEPORT: // Teleports the player vertically upwards by 100 units
			{
				Components::Transform* transform = go->getComponent<Components::Transform>();
				Utils::Vector2D pos = *transform->getPosition();
				transform->setPosition(pos.x, pos.y - 100.0f);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	void InputEvent::applyRecorderInput() {
		uint8_t inputByte = m_inputHandler.inputByte;
		if ((inputByte & INPUT_START_RECORDING)  == INPUT_START_RECORDING) {
			eventManager->raiseEvent(new StartRecordingEvent(m_timeStampPriority, m_priority));
		}
		if ((inputByte & INPUT_STOP_RECORDING) == INPUT_STOP_RECORDING) {
			eventManager->raiseEvent(new StopRecordingEvent(m_timeStampPriority, m_priority));
		}
		if ((inputByte & INPUT_PLAYBACK_RECORDING) == INPUT_PLAYBACK_RECORDING) {
			eventManager->raiseEvent(new StartPlaybackEvent(m_timeStampPriority, m_priority));
		}
	}

	void InputEvent::applyActionInput() {
		if ((m_inputHandler.inputByte & INPUT_ACTION) == INPUT_ACTION) {
			// Action code specific to game here
		}
	}
}