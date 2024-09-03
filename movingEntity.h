#include "entity.h"
#include "vector2D.h"

#pragma once
#ifndef OSCILLATINGENTITY_H
#define OSCILLATINGENTITY_H

namespace Entities {

	class MovingEntity : public Entity {
	private:
		bool m_continuous;
		bool m_reverse;
		bool m_active;
		int m_pauseTimer;
		float m_speed;
		float m_progress;
		Utils::Vector2D m_endPosition;
	public:
		MovingEntity(float scaleX, float scaleY, float positionX, float positionY, float mass,
			const char* textureFilepath, bool isStationary, bool affectedByPhysics, 
			bool continuous, bool reverse, bool active, int pauseTimer, float speed, float progress);

		void move();

		bool isContinuous();
		void toggleContinuous(bool continuous);

		bool isReverse();
		void toggleReverse(bool reverse);

		bool isActive();
		void toggleActive(bool active);

		float getSpeed();
		void setSpeed(float speed);

		float getProgress();
		void setProgress(float progress);

		Utils::Vector2D getEndPosition();
		void setEndPosition(Utils::Vector2D position);
	};

}

#endif