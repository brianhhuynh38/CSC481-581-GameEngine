#include "inputEvent.h"

#include "vector2D.h"
#include "rigidBody.h"
#include "playerInput.h"

namespace Events {

	InputEvent::InputEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, InputHandler inputHandler, Utils::Vector2D movementVector) {
		this->m_goRefVector = goRef;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
		this->m_inputHandler = inputHandler;
		this->m_movementVector = movementVector;
	}

	void InputEvent::onEvent() const {
		for (GameObject* go : m_goRefVector) {
			Utils::Vector2D moveVector = Utils::Vector2D(0, 0);
			//std::cout << "The movementVector in movePlayer: " << movementVector.toString() << "\n";
			float deltaTimeInSecs = go->getDeltaTimeInSecsOfObject();
			Utils::Vector2D posMover = Utils::Vector2D(0, 0); // amount to change positon by
			Utils::Vector2D velMover = Utils::Vector2D(0, 0); // amount to change velocity by
			float decelerationRate = 10;

			Components::Transform* transform = go->getComponent<Components::Transform>();
			Components::RigidBody* rb = go->getComponent<Components::RigidBody>();

			// Get reference to RigidBody velocity
			Utils::Vector2D* velocity = rb->getVelocity();

			// moving on x-axis
			if (m_movementVector.x != 0.0f) {

				// The vector the player will move by at the end
				moveVector.x = m_movementVector.x * deltaTimeInSecs;
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
					// if the absolute value of the player's velocity is less than 2, it will then cancel out the velocity with no decceleration.
				if (rb->getVelocity()->x > 2) {
					velMover.x = -decelerationRate * velocity->x * deltaTimeInSecs;
				}
				else if (rb->getVelocity()->x < -2) {
					velMover.x = decelerationRate * velocity->x * deltaTimeInSecs;
				}
				else {
					velMover.x = -velocity->x;
				}

				//player->updateVelocity(Utils::Vector2D((*player->getVelocity()).multConst(-1).x, 0));

			}

			posMover.x = velocity->x * deltaTimeInSecs;

			// moving on y-axis
			if (m_movementVector.y != 0.0f) {

				moveVector.y = m_movementVector.y * deltaTimeInSecs;
				velMover.y = moveVector.y;

				posMover.y = velocity->y * deltaTimeInSecs;
				//player->setVelocity( player->getVelocity()->x, moveVector.y);

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

			// move player
			rb->updateVelocity(velMover);
			transform->updatePosition(posMover);

			//std::cout << "Vel: " << velocity->x << ", " << velocity->y << "\n";
			//std::cout << "posMover: " << posMover.x << ", " << posMover.y << "\n";

			//player->updateVelocity(moveVector);
			//player->updatePosition(moveVector);

			// Move colliders to player position
			rb->setColliderCoordinates(*transform->getPosition());

			// Gets the most recent collision information frfom RigidBody
			HitInfo hInfo = rb->getMostRecentCollisionInfo();

			//std::cout << "HitInfo inside player collider: " << hInfo.hit <<
			//	"\nHitInfo direction: " << hInfo.hitVector.toString() << 
			//	"HitInfo penetration depth: " << hInfo.penetrationDepth.toString() << 
			//	"IsGrounded: " << m_isGrounded <<
			//	"\nAxis: " << axis <<"\n";


			Components::PlayerInputPlatformer* pi = go->getComponent<Components::PlayerInputPlatformer>();
			// If the object collided with something
			if (hInfo.hit && m_movementVector.y != 0) { // y-axis collision
				//std::cout << "Y-HIT\n";
				// set as grounded if player was moving down during y-axis collision
				pi->setIsGrounded(velocity->y >= 0);
				//m_isGrounded = ();
				//std::cout << "\n\nWhoa it actually got into here?? Is it grounded though?: " << m_isGrounded << "\n\n";
			}
			// if there is no collision on the y-axis, set isGrounded to false
			else if (m_movementVector.y != 0) {
				pi->setIsGrounded(false);
			}
		}
	}
}