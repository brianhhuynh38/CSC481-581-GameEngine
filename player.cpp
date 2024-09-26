#include <sstream>

#include "player.h"
#include "draw.h"
#include "vector2D.h"
#include "collisions.h"

namespace Entities {

	/**
	 * Constructs the Player and initializes all fields
	 */
	Player::Player(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
		const char* textureFilepath, bool isStationary, bool affectedByPhysics, float jumpVectorX, float jumpVectorY, float maxSpeed) {
		m_scale = new Utils::Vector2D(scaleX, scaleY);
		m_position = new Utils::Vector2D(positionX, positionY);
		m_size = new Utils::Vector2D(width, height);
		m_mass = mass;

		m_velocity = new Utils::Vector2D(0.0, 0.0);
		m_acceleration = new Utils::Vector2D(0.0, 0.0);
		m_jumpVector = new Utils::Vector2D(jumpVectorX, jumpVectorY);

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
		*m_acceleration = movementVector;

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
			//std::cout << "collided\n";
			// Move back (code below from tutorial. Replace with code that fits our setup)
			//std::cout << "Player collision pos: " << m_position->x << ", " << m_position->y << "\n";
			//*m_position = oldPosition;
			updatePosition(movementVector.multConst(-1));
			//updateVelocity((*m_velocity).multConst(-1));
			//m_position->add(movementVector.multConst(-1));
			/**m_velocity = m_velocity->add((*m_velocity).multConst(-1));
			*m_acceleration = m_acceleration->add((*m_acceleration).multConst(-1));*/
			//std::cout << "Player new pos: " << m_position->x << ", " << m_position->y << "\n";

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
		*m_jumpVector = jVector;
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

	std::string Player::toString() {
		// Create string to serialize all fields within the Entity
		// Each field is delineated by a newline
		std::stringstream ss;
		// Stringify ID
		ss << m_uuid << "\n";
		// Stringify positional and physics vectors
		ss << getPosition()->toString();
		ss << getVelocity()->toString();
		ss << getAcceleration()->toString();
		// Stringify scale, size, and mass
		ss << getScale()->toString();
		ss << getSize()->toString();
		ss << getMass() << "\n";
		// Stringify max velocity and acceleration values
		ss << m_velocity_max << "\n";
		ss << m_acceleration_max << "\n";
		// Get filepath to SDLTexture
		ss << m_textureFilepath << "\n";
		// Stringifies each SDL_Rect Collider
		for (SDL_Rect collider : *m_colliders) {
			ss << collider.x << "," << collider.y << "," << collider.w << "," << collider.h << "\t";
		}
		ss << "\n";
		// Stringifies each of the bools: stationary, affectedByPhysics, visible
		ss << m_isStationary << "," << m_affectedByPhysics << "," << m_isVisible << "\n";
		//Stringify Player-specific stuff
		ss << getMaxSpeed() << "," << getIsGrounded() << "," << getJumpVector()->toString();
		return ss.str();
	}
}
