#include "entity.h"
#include "vector2D.h"

#pragma once
#ifndef MOVINGENTITY_H
#define MOVINGENTITY_H

namespace Entities {

	/**
	* An entity that moves from Point A to Point B horizontally.
	* This might be expanded to move multidirectionally in the future.
	* MovingEntity inherits from the superclass Entity
	*/
	class MovingEntity : public Entity {
	private:
		/** Whether or not the object is continuously moving */
		bool m_continuous;
		/** Whether or not the object is moving from end to start */
		bool m_reverse;
		/** The amount of time that the object will pause for at the end of its path */
		int m_pauseTimer;
		/** The timer that counts down before the entity starts moving again */
		int m_currentTimer;
		/** The speed at which the entity moves */
		float m_speed;
		/** The entity's starting position */
		Utils::Vector2D m_startPosition;
		/** The entity's destination */
		Utils::Vector2D m_endPosition;
	public:

		/**
		 * Default constructor
		 */
		MovingEntity();

		/**
		* The constructor for MovingEntity
		* 
		* @param scaleX X component of scaling multipliers for rendering
		* @param scaleY Y component of scaling multipliers for rendering
		* @param position The coordinates where the Entity is located
		* @param size The size of the Entity
		* @param mass The mass of the Entity
		* @param textureFilepath The filepath to where the texture is located
		* @param isStationary Whether the object should move
		* @param affectedByPhysics Whether the object is affectedByPhysics
		* @param continuous Whether or not the object is continuously moving
		* @param reverse Whether or not the object is reversing
		* @param pauseTimer The amount of time the object will stop at the end or start
		* @param speed The speed the entity moves at
		* @param endPosX The x component of the end position
		* @param endPosY The y component of the end position
		*/
		MovingEntity(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
			const char* textureFilepath, bool isStationary, bool affectedByPhysics, 
			bool continuous, bool reverse, int pauseTimer, float speed, float endPosX, float endPosY);

		/**
		* Forcibly moves the entity left or right
		*/
		void move();

		/**
		* Sets the position of the entity given a percentage float
		* 
		* @param progress: The percentage of the way from the start to the end (0-1.0)
		*/
		void setProgress(float progress);

		/**
		* Returns true if continuous
		*/
		bool isContinuous();

		/**
		* Toggles continuous to be true or false
		*/
		void toggleContinuous(bool continuous);

		/**
		* Returns true if path is reversed
		*/
		bool isReverse();

		/**
		* Toggles reverse to be true or false
		*/
		void toggleReverse(bool reverse);

		/**
		* Returns the entity's speed
		*/
		float getSpeed();

		/**
		* Sets the speed at which the entity moves
		*/
		void setSpeed(float speed);

		/**
		* Creates a string containing all information for the MovingEntity
		*/
		std::string toString();
	};

}

#endif