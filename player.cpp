#include "player.h"

#include "draw.h"
#include "vector2D.h"

namespace Entities {

	/**
	* Constructs the Player and initializes all fields
	*/
	Player::Player(float scaleX, float scaleY, float positionX, float positionY, float mass,
		const char* textureFilepath, bool isStationary, bool affectedByPhysics, float maxSpeed) {
		m_scale = new Utils::Vector2D(scaleX, scaleY);
		m_position = new Utils::Vector2D(positionX, positionY);
		m_velocity = new Utils::Vector2D(0.0, 0.0);
		m_acceleration = new Utils::Vector2D(0.0, 0.0);
		m_mass = mass;
		m_texture = Render::loadTexture(textureFilepath);
		m_isStationary = isStationary;
		m_affectedByPhysics = affectedByPhysics;
		m_maxSpeed = maxSpeed;
	}

	/**
	* Moves the player by a given amount by manipulating its velocity vector
	* @param movementVector The vector that is changes the player's movement
	*/
	void Player::move(Utils::Vector2D movementVector) {
		*m_position = m_position->add(movementVector);
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
