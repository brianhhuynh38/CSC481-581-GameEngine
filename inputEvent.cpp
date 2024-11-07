#include "inputEvent.h"

#include "vector2D.h"

namespace Events {

	InputEvent::InputEvent(std::vector<GameObject*> goRefVector, int64_t timeStampPriority, int priority, int8_t inputByte) {
		this->m_goRefVector = goRefVector;
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
		this->m_inputByte = inputByte;
	}

	void InputEvent::onEvent() const {
		for (GameObject* go : m_goRefVector) {
			Utils::Vector2D moveVector;
			//std::cout << "The movementVector in movePlayer: " << movementVector.toString() << "\n";
			float deltaTimeInSecs = go->getDeltaTimeInSecsOfObject();
			Utils::Vector2D posMover = Utils::Vector2D(0, 0); // amount to change positon by
			Utils::Vector2D velMover = Utils::Vector2D(0, 0); // amount to change velocity by
			float decelerationRate = 10;

			// Get reference to RigidBody velocity
			Utils::Vector2D* velocity = rb->getVelocity();

			// moving on x-axis
			if (axis == 0) {

				// The vector the player will move by at the end
				moveVector = Utils::Vector2D(movementVector.x * deltaTimeInSecs, 0);

				if (movementVector.x == 0) { // decelerate-x
					// moves velocity towards zero.
					// if the absolute value of the player's velocity is less than 2, it will then cancel out the velocity with no decceleration.
					if (rb->getVelocity()->x > 2) {
						velMover = Utils::Vector2D((-decelerationRate * Utils::Vector2D(velocity->x, 0).multConst(deltaTimeInSecs).getMagnitude()), 0);
					}
					else if (rb->getVelocity()->x < -2) {
						velMover = Utils::Vector2D((decelerationRate * Utils::Vector2D(velocity->x, 0).multConst(deltaTimeInSecs).getMagnitude()), 0);
					}
					else {
						velMover = Utils::Vector2D(-1 * velocity->x, 0);
					}

					//player->updateVelocity(Utils::Vector2D((*player->getVelocity()).multConst(-1).x, 0));

				}
				else { // accelerate-x
					// check if changing directions
					if ((movementVector.x > 0 && velocity->x < 0) || (movementVector.x < 0 && velocity->x > 0)) {
						// increase rate of accelaration until moving in desired direction
						velMover = moveVector.multConst(decelerationRate);
					}
					else {
						velMover = moveVector;
					}

				}

				rb->updateVelocity(velMover);
				posMover = Utils::Vector2D(velocity->x, 0).multConst(deltaTimeInSecs);
				//player->setVelocity(moveVector.x, player->getVelocity()->y);

			}
			// moving on y-axis
			else if (axis == 1) {

				moveVector = Utils::Vector2D(0, movementVector.y * deltaTimeInSecs);

				if (movementVector.y == 0) { // decelerate-y
					// moves velocity towards zero.
					// if the absolute value of the player's velocity is less than 2, it will then cancel out the velocity with no decceleration.
					if (velocity->y > 2) {
						velMover = Utils::Vector2D(0, (-5 * Utils::Vector2D(0, velocity->y).multConst(deltaTimeInSecs).getMagnitude()));
					}
					else if (velocity->y < -2) {
						velMover = Utils::Vector2D(0, (5 * Utils::Vector2D(0, velocity->y).multConst(deltaTimeInSecs).getMagnitude()));
					}
					else {
						velMover = Utils::Vector2D(0, -1 * velocity->y);
					}

					//player->updateVelocity(Utils::Vector2D(0, (*player->getVelocity()).multConst(-1).y));

				}
				else { // accelerate-y
					velMover = moveVector;

				}

				rb->updateVelocity(velMover);
				posMover = Utils::Vector2D(0, velocity->y).multConst(deltaTimeInSecs);
				//player->setVelocity( player->getVelocity()->x, moveVector.y);

			}
			else {
				moveVector = Utils::Vector2D(0, 0);
			}

			// move player
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

			// If the object collided with something
			if (hInfo.hit && axis == 1) { // y-axis collision
				//std::cout << "Y-HIT\n";
				// set as grounded if player was moving down during y-axis collision
				m_isGrounded = (velocity->y >= 0);
				//std::cout << "\n\nWhoa it actually got into here?? Is it grounded though?: " << m_isGrounded << "\n\n";
			}
			// if there is no collision on the y-axis, set isGrounded to false
			else if (axis == 1) {
				m_isGrounded = false;
			}
		}
		}
	}
}