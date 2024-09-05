#include "player.h"

#include "draw.h"
#include "vector2D.h"
#include "collisions.h"

namespace Entities {

	/**
	 * Constructs the Player and initializes all fields
	 */
	Player::Player(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
		const char* textureFilepath, bool isStationary, bool affectedByPhysics, float maxSpeed) {
		m_scale = new Utils::Vector2D(scaleX, scaleY);
		m_position = new Utils::Vector2D(positionX, positionY);
		m_size = new Utils::Vector2D(width, height);
		m_mass = mass;

		m_velocity = new Utils::Vector2D(0.0, 0.0);
		m_acceleration = new Utils::Vector2D(0.0, 0.0);
		m_jumpVector = new Utils::Vector2D(0.0, -6.0);

		m_texture = Render::loadTexture(textureFilepath);

		m_isStationary = isStationary;
		m_affectedByPhysics = affectedByPhysics;
		m_maxSpeed = maxSpeed;
		isGrounded = false;
		m_colliders = new std::list<SDL_Rect>();
		m_colliders->emplace_back(SDL_Rect() = {(int) positionX, (int) positionY, (int) (scaleX * width), (int) (scaleY * height)});
	}

	/**
	 * Moves the player by a given amount by manipulating its velocity vector
	 * @param movementVector The vector that is changes the player's movement
	 */
	void Player::move(Utils::Vector2D movementVector) {
		Utils::Vector2D oldPosition = *m_position;

		// quick fix for geting to fast
		float maxMove = 10;
		if (movementVector.x > maxMove) {
			movementVector.x = maxMove;
		}
		else if (movementVector.x < -maxMove) {
			movementVector.x = -maxMove;
		}
		if (movementVector.y > maxMove) {
			movementVector.y = maxMove;
		}
		else if (movementVector.y < -maxMove) {
			movementVector.y = -maxMove;
		}

		// update velocity
		*m_velocity = movementVector;

		*m_position = m_position->add(movementVector);

		// Create colliders iterator
        std::list<SDL_Rect>::iterator iterCol;

		//std::cout << "Player coordinates: " << m_position->x << ", " << m_position->y << "\n";
        // Loop through colliders
        for (iterCol = m_colliders->begin(); iterCol != m_colliders->end(); ++iterCol) {
			// Move the colliders the calculated distance
			iterCol->x = m_position->x;
			iterCol->y = m_position->y;
			//std::cout << "Collider coordinates: " << iterCol->x << "," << iterCol->y << "\n";
		}

		// Checks for collisions
		HitInfo hInfo = checkCollisions(m_colliders, entityController->getEntities());

		// If the object collided with something
		if( hInfo.hit ) {
			std::cout << "collided\n";
			// Move back (code below from tutorial. Replace with code that fits our setup)
			*m_position = oldPosition;
			
			//std::cout << "collided at: " << m_position->x << ", " << m_position->y << "\n";
			// pushes away from collision box 
			/*if ( abs(hInfo.hitVector.x) > abs(hInfo.hitVector.y) ) {
				*m_position = m_position->add(Utils::Vector2D(-m_velocity->x, 0));
				std::cout << "pushed by: " << -m_velocity->x << ", " << 0 << "\n";
			}
			else {
				*m_position = m_position->add(Utils::Vector2D(0, -m_velocity->y));
				std::cout << "pushed by: " << 0 << ", " << -m_velocity->y << "\n";
			}*/
			//std::cout << "now at: " << m_position->x << ", " << m_position->y << "\n";


			// Create colliders iterator
        	std::list<SDL_Rect>::iterator iterCol2;

			for (iterCol2 = m_colliders->begin(); iterCol2 != m_colliders->end(); ++iterCol2) {
				// Move the colliders the calculated distance
				//iterCol2->x = oldPosition.x;
				//iterCol2->y = oldPosition.y;
				iterCol2->x = m_position->x;
				iterCol2->y = m_position->y;
		
				//std::cout << "COLLIDED coordinates: " << iterCol2->x << "," << iterCol2->y << "\n";
			}

			// set as grounded if 
			isGrounded = m_velocity->y > 0;
		}
	}

	/**
	 * Returns the player's max speed
	 * @returns The maximum amount of speed the object can go
	 */
	float Player::getMaxSpeed(void) {
        return m_maxSpeed;
	}

	/**
	 * Change the maximum speed the player can go
	 */
	void Player::setMaxSpeed(float maxSpeed) {
        m_maxSpeed = maxSpeed;
	}

	/**
	 * Returns jumpVector of player
	 * @return jumpVector of player
	 */
	Utils::Vector2D* Player::getJumpVector() {
		return m_jumpVector;
	}

	/**
	 * Sets players jumpVector value to the given
	 * @param jumpVector value
	 */
	void Player::setJumpVector(Utils::Vector2D jVector) {
		m_jumpVector = &jVector;
	}

	/**
	 * Returns true if on ground
	 * @return true if on ground
	 */
	bool Player::getIsGrounded() {
		return isGrounded;
	}

	/**
	 * sets players isGrounded value to the given
	 * @param grounded value
	 */
	void Player::setIsGrounded(bool grounded) {
		isGrounded = grounded;
	}
}
