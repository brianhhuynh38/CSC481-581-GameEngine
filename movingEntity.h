#include "entity.h"
#include "vector2D.h"

#pragma once
#ifndef MOVINGENTITY_H
#define MOVINGENTITY_H

namespace Entities {

	/**
	* An entity that moves from Point A to Point B horizontally.
	* This might be expanded to move multidirectionally in the future
	*/
	class MovingEntity : public Entity {
	private:
		bool m_continuous;
		bool m_reverse;
		int m_pauseTimer;
		int m_currentTimer;
		float m_speed;
		Utils::Vector2D m_startPosition;
		Utils::Vector2D m_endPosition;
	public:
		MovingEntity(float scaleX, float scaleY, float positionX, float positionY, float mass,
			const char* textureFilepath, bool isStationary, bool affectedByPhysics, 
			bool continuous, bool reverse, int pauseTimer, float speed, float endPosX, float endPosY);

		void move();

		void setProgress(float progress);

		bool isContinuous();
		void toggleContinuous(bool continuous);

		bool isReverse();
		void toggleReverse(bool reverse);

		float getSpeed();
		void setSpeed(float speed);

		Utils::Vector2D getEndPosition();
		void setEndPosition(Utils::Vector2D position);
	};

}

#endif