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
		m_isGrounded = false;
		m_colliders = new std::list<SDL_Rect>();
		m_colliders->emplace_back(SDL_Rect() = {(int) positionX, (int) positionY, (int) (scaleX * width), (int) (scaleY * height)});
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

	
}
