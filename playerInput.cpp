#include "playerInput.h"

#include "component.h"
#include "transform.h"
#include "rigidBody.h"
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
		m_canPressInput = true;
	}

	PlayerInputPlatformer::PlayerInputPlatformer(float maxMovementSpeed, Utils::Vector2D jumpVector, InputHandler* inputHandler, GameObject* parentRef) {
		m_maxMovementSpeed = maxMovementSpeed;
		m_jumpVector = jumpVector;
		m_inputHandler = inputHandler;
		m_parent = parentRef;
		m_canPressInput = true;
	}

	// Helper method?


	void PlayerInputPlatformer::update() {
		if (m_canPressInput) {

			Transform *transform = m_parent->getComponent<Transform>();
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
			// Jump
			if (m_inputHandler->keyboard[SDL_SCANCODE_UP] == 1 && m_isGrounded) {
				moveVector.y = m_jumpVector.y;
				m_isGrounded = false;
				std::cout << "JUMPER: " << moveVector.y << "\n";
			}
			else {
				// gravity
				moveVector.y += rb->getAcceleration()->y;
			}

			// moveVector.multConst(m_timeline->getDeltaTime() / MICROSEC_PER_SEC);

			//player->updateVelocity(moveVector);


			std::cout << "moveVector: " << moveVector.x << ", " << moveVector.y << "\n";

			// move on x axis
			movePlayer(Utils::Vector2D(moveVector.x, NULL), 0);
			// move on y axis
			movePlayer(Utils::Vector2D(NULL, moveVector.y), 1);
		}
	}

	void PlayerInputPlatformer::movePlayer(Utils::Vector2D movementVector, int axis) {
		Utils::Vector2D moveVector;

		float deltaTimeInSecs = static_cast<float>(m_timeline->getDeltaTime()) / MICROSEC_PER_SEC;
		Utils::Vector2D posMover = Utils::Vector2D(0, 0); // amount to change positon by
		Utils::Vector2D velMover = Utils::Vector2D(0, 0); // amount to change velocity by
		float decelerationRate = 10;

		// moving on x-axis
		if (axis == 0) {

			moveVector = Utils::Vector2D(movementVector.x * deltaTimeInSecs, 0);

			if (movementVector.x == 0) { // decelerate-x
				// moves velocity towards zero.
				// if the absolute value of the player's velocity is less than 2, it will then cancel out the velocity with no decceleration.
				if (player->getVelocity()->x > 2) {
					velMover = Utils::Vector2D((-decelerationRate * Utils::Vector2D(player->getVelocity()->x, 0).multConst(deltaTimeInSecs).getMagnitude()), 0);
				}
				else if (player->getVelocity()->x < -2) {
					velMover = Utils::Vector2D((decelerationRate * Utils::Vector2D(player->getVelocity()->x, 0).multConst(deltaTimeInSecs).getMagnitude()), 0);
				}
				else {
					velMover = Utils::Vector2D(-1 * player->getVelocity()->x, 0);
				}

				//player->updateVelocity(Utils::Vector2D((*player->getVelocity()).multConst(-1).x, 0));

			}
			else { // accelerate-x
				// check if changing directions
				if ((movementVector.x > 0 && player->getVelocity()->x < 0) || (movementVector.x < 0 && player->getVelocity()->x > 0)) {
					// increase rate of accelaration until moving in desired direction
					velMover = moveVector.multConst(decelerationRate);
				}
				else {
					velMover = moveVector;
				}

			}

			player->updateVelocity(velMover);
			posMover = Utils::Vector2D(player->getVelocity()->x, 0).multConst(deltaTimeInSecs);
			//player->setVelocity(moveVector.x, player->getVelocity()->y);

		}
		// moving on y-axis
		else if (axis == 1) {

			moveVector = Utils::Vector2D(0, movementVector.y * deltaTimeInSecs);

			if (movementVector.y == 0) { // decelerate-y
				// moves velocity towards zero.
				// if the absolute value of the player's velocity is less than 2, it will then cancel out the velocity with no decceleration.
				if (player->getVelocity()->y > 2) {
					velMover = Utils::Vector2D(0, (-5 * Utils::Vector2D(0, player->getVelocity()->y).multConst(deltaTimeInSecs).getMagnitude()));
				}
				else if (player->getVelocity()->y < -2) {
					velMover = Utils::Vector2D(0, (5 * Utils::Vector2D(0, player->getVelocity()->y).multConst(deltaTimeInSecs).getMagnitude()));
				}
				else {
					velMover = Utils::Vector2D(0, -1 * player->getVelocity()->y);
				}

				//player->updateVelocity(Utils::Vector2D(0, (*player->getVelocity()).multConst(-1).y));

			}
			else { // accelerate-y
				velMover = moveVector;

			}

			player->updateVelocity(velMover);
			posMover = Utils::Vector2D(0, player->getVelocity()->y).multConst(deltaTimeInSecs);
			//player->setVelocity( player->getVelocity()->x, moveVector.y);

		}
		else {
			moveVector = Utils::Vector2D(0, 0);
		}

		// move player
		player->updatePosition(posMover);

		std::cout << "Vel: " << player->getVelocity()->x << ", " << player->getVelocity()->y << "\n";
		std::cout << "posMover: " << posMover.x << ", " << posMover.y << "\n";

		//player->updateVelocity(moveVector);
		//player->updatePosition(moveVector);

		// Create colliders iterator
		std::list<SDL_Rect>::iterator iterCol;

		std::map<int, Entities::Entity> entityMap = *entityController->getEntities();

		//std::cout << "Player coordinates: " << m_position->x << ", " << m_position->y << "\n";
		// Loop through colliders
		for (iterCol = player->getColliders()->begin(); iterCol != player->getColliders()->end(); ++iterCol) {
			// Move the colliders the calculated distance
			iterCol->x = player->getPosition()->x;
			iterCol->y = player->getPosition()->y;
			//std::cout << "Collider coordinates: " << iterCol->x << "," << iterCol->y << "\n";
		}

		// Checks for collisions
		HitInfo hInfo = checkCollisions(player->getColliders(), &entityMap);

		// If the object collided with something
		if (hInfo.hit) {
			if (axis == 0) { // x-axis collision
				//std::cout << "X-HIT\n";
				player->updatePosition(posMover.multConst(-1));
				player->updateVelocity(Utils::Vector2D(-1 * player->getVelocity()->x, 0));
			}
			else if (axis == 1) { // y-axis collision
				//std::cout << "Y-HIT\n";

				// set as grounded if player was moving down during y-axis collision
				player->setIsGrounded(player->getVelocity()->y >= 0);

				// update position and velocity of player
				player->updatePosition(posMover.multConst(-1));
				player->updateVelocity(Utils::Vector2D(0, -1 * player->getVelocity()->y));

			}

			// Create colliders iterator
			std::list<SDL_Rect>::iterator iterCol2;

			for (iterCol2 = player->getColliders()->begin(); iterCol2 != player->getColliders()->end(); ++iterCol2) {

				iterCol2->x = player->getPosition()->x;
				iterCol2->y = player->getPosition()->y;

				//std::cout << "COLLIDED coordinates: " << iterCol2->x << "," << iterCol2->y << "\n";
			}


		}
		// if there is no collision on the y-axis, set isGrounded to false
		else if (axis == 1) {
			player->setIsGrounded(false);

		}
	}
}
