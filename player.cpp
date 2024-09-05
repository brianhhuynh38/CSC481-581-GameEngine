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
		m_velocity = new Utils::Vector2D(0.0, 0.0);
		m_acceleration = new Utils::Vector2D(0.0, 0.0);
		m_mass = mass;
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

		// If the object collided
		if( checkCollisions(m_colliders, entityController->getEntities()) ) {
			std::cout << "collided\n";
			// Move back (code below from tutorial. Replace with code that fits our setup)
			*m_position = oldPosition;

			// Create colliders iterator
        	std::list<SDL_Rect>::iterator iterCol2;

			for (iterCol2 = m_colliders->begin(); iterCol2 != m_colliders->end(); ++iterCol2) {
				// Move the colliders the calculated distance
				iterCol2->x = oldPosition.x;
				iterCol2->y = oldPosition.y;
				//std::cout << "COLLIDED coordinates: " << iterCol2->x << "," << iterCol2->y << "\n";
			}
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
}
