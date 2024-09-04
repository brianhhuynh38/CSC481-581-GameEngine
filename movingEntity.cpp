#include <SDL.h>
#include <cmath>

#include "movingEntity.h"
#include "draw.h"
#include "global.h"

namespace Entities {

	/**
	* The constructor for the moving entity. This implementation moves from left to right.
	* 
	* @param scaleX: The x component of the object's scale
	* @param scaleY: The y component of the object's scale
	* @param positionX: The x component of the Object's position
	* @param positionY: The y component of the Object's position
	* @param mass: The mass of the object
	* @param textureFilepath: The filepath to the object's texture
	* @param isStationary: Whether or not the object is able to move
	* @param affectedByPhysics: Whether or not the object is affected by physics
	* @param continuous: Whether the object constantly repeats and reverses its movement
	* @param reverse: Determines if the object moves from end to beginning or beginning to end
	* @param pauseTimer: How long the entity stops for when reaching its destination before moving again
	* @param speed: The speed at which the entity moves
	* @param progress: The progress the object has made through its path, expressed as a decimal
	* @param endPosX: The x component of the position at the end of the object's path
	* @param endPosY: The y component of the position at the end of the object's path
	*/
	MovingEntity::MovingEntity(float scaleX, float scaleY, float positionX, float positionY, float mass,
		const char* textureFilepath, bool isStationary, bool affectedByPhysics,
		bool continuous, bool reverse, int pauseTimer, float speed, float endPosX, float endPosY) {
		// Entity parts
		m_scale = new Utils::Vector2D(scaleX, scaleY);
		m_position = new Utils::Vector2D(positionX, positionY);
		m_velocity = new Utils::Vector2D(0.0, 0.0);
		m_acceleration = new Utils::Vector2D(0.0, 0.0);
		m_mass = mass;
		m_texture = Render::loadTexture(textureFilepath);
		m_isStationary = isStationary;
		m_affectedByPhysics = affectedByPhysics;
		// MovingEntity parts
		m_continuous = continuous;
		m_reverse = reverse;
		m_pauseTimer = pauseTimer;
		m_currentTimer = 0;
		m_speed = speed;
		m_startPosition = Utils::Vector2D(positionX, positionY);
		m_endPosition = Utils::Vector2D(endPosX, endPosY);
	}

	/**
	* Forcibly moves the object to or from its end position or beginning position
	* TODO: Modify to allow for multidirectional movement in the future
	*/
	void MovingEntity::move() {
		// Find distance that is travelled within the allotted deltaTime
		float distance = m_speed * (timeline.getDeltaTime() / 1000);
		// Move the object if it is not frozen and is not paused by the timer
		if (!m_isStationary && m_currentTimer <= 0) {
			// Move the object back to the start position
			if (m_reverse) {
				// Move the object the calculated distance
				m_position->x -= distance;
				// If the object moves past the start position, set to lower bound, then set currentTimer
				if (m_position->x < m_startPosition.x) {
					m_position->x = m_startPosition.x;
					m_currentTimer = m_pauseTimer;
				}
				// If the object collided or went too far to the left or right
				if( ( m_position->x < 0 ) || ( m_position->x + m_scale > SCREEN_WIDTH ) || checkCollisions( m_colliders, m_entities ) ) {
					// Move back (code below from tutorial. Replace with code that fits our setup)
					// mPosX -= mVelX;
        			// mCollider.x = mPosX;
				}
			}
			else { // Move the object towards the end position
				// Move the object the calculated distance
				m_position->x += distance;
				// If the object moves past the end position, set to upper bound, then set currentTimer
				if (m_position->x > m_endPosition.x) {
					m_position->x = m_endPosition.x;
					m_currentTimer = m_pauseTimer;
				}
				// If the object collided or went too far up or down
				if( ( m_position->y < 0 ) || ( m_position->y + m_scale > SCREEN_HEIGHT ) || checkCollisions( m_colliders, m_entities ) ) {
					// Move back (code below from tutorial. Replace with code that fits our setup)
					// mPosY -= mVelY;
        			// mCollider.y = mPosY;
				}
			}
		}
		// Decrement timer if there is a timer active
		if (m_currentTimer > 0) {
			m_currentTimer--;
		}
	}

	/**
	* Changes the position of the object based on the progress float percentage provided that is between
	* 0 and 1. A value provided under 0 or over 1 will make the object move to the starting or end positions
	* respectively.
	*
	* @param progress: The amount of progress made towards the end goal, expressed as a float between 0-1
	*/
	void MovingEntity::setProgress(float progress) {
		if (progress <= 0) {
			// Set to the starting position if 0% or lower and start timer
			m_position->x = m_startPosition.x;
			m_currentTimer = m_pauseTimer;
		}
		else if (progress >= 1) {
			// Set to end position if 100% or over and start timer
			m_position->x = m_endPosition.x;
			m_currentTimer = m_pauseTimer;
		}
		else {
			// Change position based on given progress check
			m_position->x += progress * (m_endPosition.x - m_position->x);
		}
	}

	bool MovingEntity::isContinuous() {
		return m_continuous;
	}

	void MovingEntity::toggleContinuous(bool continuous) {
		m_continuous = continuous;
	}

	bool MovingEntity::isReverse() {
		return m_reverse;
	}

	void MovingEntity::toggleReverse(bool reverse) {
		m_reverse = reverse;
	}

	float MovingEntity::getSpeed() {
		return m_speed;
	}

	void MovingEntity::setSpeed(float speed) {
		m_speed = speed;
	}

	Utils::Vector2D MovingEntity::getEndPosition() {
		return m_endPosition;
	}

	void MovingEntity::setEndPosition(Utils::Vector2D position) {
		m_endPosition = position;
	}
}
