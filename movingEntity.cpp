#include <SDL.h>
#include <cmath>

#include "movingEntity.h"
#include "draw.h"
#include "global.h"
#include "definitions.h"
#include "collisions.h"
#include "stage.h"

namespace Entities {

	MovingEntity::MovingEntity() {
		// Entity parts
		m_scale = new Utils::Vector2D(1, 1);
		m_position = new Utils::Vector2D(300, 300);
		m_size = new Utils::Vector2D(1, 1);
		m_velocity = new Utils::Vector2D(0.0, 0.0);
		m_acceleration = new Utils::Vector2D(0.0, 0.0);
		m_mass = 1;
		m_texture = Render::loadTexture(".\Assets\Textures\MissingTexture.png");
		m_isStationary = false;
		m_affectedByPhysics = true;
		// MovingEntity parts
		m_continuous = true;
		m_reverse = false;
		m_pauseTimer = 10;
		m_currentTimer = 0;
		m_speed = 5;
		m_startPosition = Utils::Vector2D(300, 300);
		m_endPosition = Utils::Vector2D(800, 500);
		m_colliders = new std::list<SDL_Rect>();
		m_colliders->emplace_back(SDL_Rect() = { (int)m_position->x, (int)m_position->y, (int)(m_scale->x * m_size->x), (int)(m_scale->y * m_size->y) });
	}

	/**
	* The constructor for the moving entity. This implementation moves from left to right.
	* 
	* @param scaleX: The x component of the object's scale
	* @param scaleY: The y component of the object's scale
	* @param positionX: The x component of the Object's position
	* @param positionY: The y component of the Object's position
	* @param width: The width of the Object
	* @param height: The height component of the Object
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
	MovingEntity::MovingEntity(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
		const char* textureFilepath, bool isStationary, bool affectedByPhysics,
		bool continuous, bool reverse, int pauseTimer, float speed, float endPosX, float endPosY) {
		// Entity parts
		m_scale = new Utils::Vector2D(scaleX, scaleY);
		m_position = new Utils::Vector2D(positionX, positionY);
		m_size = new Utils::Vector2D(width, height);
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
		m_colliders = new std::list<SDL_Rect>();
		m_colliders->emplace_back(SDL_Rect() = {(int) positionX, (int) positionY, (int) (scaleX * width), (int) (scaleY * height)});
	}

	/**
	* Forcibly moves the object to or from its end position or beginning position
	* TODO: Modify to allow for multidirectional movement in the future
	*/
	void MovingEntity::move() {
		// Find distance that is travelled within the allotted deltaTime
		float distance = m_speed * (float) (timeline.getDeltaTime());
		// Move the object if it is not frozen and is not paused by the timer
		if (!m_isStationary && m_currentTimer <= 0) {

			// std::cout << "Entity: " << m_position->x << "Collider: " << m_colliders->front().x;

			// Move the object back to the start position
			if (m_reverse) {
				// Move the object the calculated distance
				m_position->x -= distance;

				// Create colliders iterator
        		std::list<SDL_Rect>::iterator iterCol;

				// If the object moves past the start position, set to lower bound, then set currentTimer
				if (m_position->x < m_startPosition.x) {
					m_position->x = m_startPosition.x;
					m_currentTimer = m_pauseTimer;
					m_reverse = false;
				}

				// Loop through colliders
        		for (iterCol = m_colliders->begin(); iterCol != m_colliders->end(); ++iterCol) {
					// Move the colliders the calculated distance
					iterCol->x = m_position->x;
				}

				HitInfo hInfo = checkCollisions(m_colliders, entityController->getEntities());
				// If the object collided going left or right
				if( hInfo.hit ) {
					// Move back (code below from tutorial. Replace with code that fits our setup)
					m_position->x += distance;

					// Create colliders iterator
        			std::list<SDL_Rect>::iterator iterCol2;

					// Move the colliders back the calculated distance
					for (iterCol2 = m_colliders->begin(); iterCol2 != m_colliders->end(); ++iterCol2) {
						iterCol2->x += distance;
					}
				}
			}
			else { // Move the object towards the end position
				// Move the object the calculated distance
				m_position->x += distance;
				
				// Create colliders iterator
        		std::list<SDL_Rect>::iterator iterCol;

				// If the object moves past the end position, set to upper bound, then set currentTimer
				if (m_position->x > m_endPosition.x) {
					m_position->x = m_endPosition.x;
					m_currentTimer = m_pauseTimer;
					m_reverse = true;
				}

				// Loop through colliders
        		for (iterCol = m_colliders->begin(); iterCol != m_colliders->end(); ++iterCol) {
					// Move the colliders the calculated distance
					iterCol->x = m_position->x;
				}

				HitInfo hInfo = checkCollisions(m_colliders, entityController->getEntities());
				
				// If the object collided going up or down
				if ( hInfo.hit ) {
					// Move back (code below from tutorial. Replace with code that fits our setup)
					m_position->y -= distance;
					// Create colliders iterator
        			std::list<SDL_Rect>::iterator iterCol2;

					// Move the colliders back the calculated distance
					for (iterCol2 = m_colliders->begin(); iterCol2 != m_colliders->end(); ++iterCol2) {
						iterCol2->y -= distance;
					}
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
}
