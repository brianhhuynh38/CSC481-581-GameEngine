#include "playerInput.h"

#include "GameObject.h"
#include "physicsCalculator.h"

#include <SDL.h>

namespace Components {

	PlayerInputPlatformer::PlayerInputPlatformer() {
		m_maxMovementSpeed = 0;
		m_jumpVector = Utils::Vector2D(1, 1);
		m_parent = nullptr;
		m_canPressInput = true;
		m_isGrounded = false;
	}

	PlayerInputPlatformer::PlayerInputPlatformer(float maxMovementSpeed, Utils::Vector2D jumpVector, GameObject* parentRef) {
		m_maxMovementSpeed = maxMovementSpeed;
		m_jumpVector = jumpVector;
		m_parent = parentRef;
		m_canPressInput = true;
		m_isGrounded = false;
	}

	void PlayerInputPlatformer::movePlayer(Utils::Vector2D movementVector, int axis, RigidBody* rb, Transform* transform) {
		Utils::Vector2D moveVector;
		//std::cout << "The movementVector in movePlayer: " << movementVector.toString() << "\n";
		float deltaTimeInSecs = m_parent->getDeltaTimeInSecsOfObject();
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

	void PlayerInputPlatformer::update() {
		if (m_canPressInput && m_inputHandler) {

			//std::cout << "Entered Player Input, is grounded?: " << m_isGrounded << "\n";

			Transform* transform = m_parent->getComponent<Transform>();
			RigidBody* rb = m_parent->getComponent<RigidBody>();

			bool isPhysicsBased = rb && !rb->isKinematic();

			Utils::Vector2D moveVector = Utils::Vector2D(0, 0);

			// horizontal movement
			if (m_inputHandler->keyboard[SDL_SCANCODE_RIGHT] == 1) {
				if (isPhysicsBased) {
					moveVector.x += m_maxMovementSpeed;
				}
				else {
					moveVector.x = m_maxMovementSpeed;
				}
			}
			if (m_inputHandler->keyboard[SDL_SCANCODE_LEFT] == 1) {
				if (isPhysicsBased) {
					moveVector.x -= m_maxMovementSpeed;
				}
				else {
					moveVector.x = -m_maxMovementSpeed;
				}
			}

			if (m_inputHandler->keyboard[SDL_SCANCODE_DOWN] == 1) {
				// nothing
			}

			//std::cout << "Does it get the up input?: " << (m_inputHandler->keyboard[SDL_SCANCODE_UP] == 1 && m_isGrounded) << "\n";
			
			// Jump
			if (m_inputHandler->keyboard[SDL_SCANCODE_UP] == 1 && m_isGrounded) {
				//std::cout << "\n\nIt entered the jump stuff\n\n\n";
				moveVector.y = m_jumpVector.y;
				m_isGrounded = false;
				//std::cout << "\n\n\nJUMPER: " << moveVector.y << "\n\n\n";
			}
			else {
				// gravity
				moveVector.y += rb->getAcceleration()->y;
			}

			// moveVector.multConst(m_timeline->getDeltaTime() / MICROSEC_PER_SEC);

			//player->updateVelocity(moveVector);


			//std::cout << "moveVector: " << moveVector.x << ", " << moveVector.y << "\n";

			// move on x axis
			movePlayer(Utils::Vector2D(moveVector.x, NULL), 0, rb, transform);
			// move on y axis
			movePlayer(Utils::Vector2D(NULL, moveVector.y), 1, rb, transform);
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
