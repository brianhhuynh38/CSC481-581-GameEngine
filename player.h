#include "vector2D.h"
#include "entity.h"

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

namespace Entities {

	class Player : public Entity {
	private:
		/** Max speed at which the player moves*/
        float m_maxSpeed;
	public:
		Player(float scaleX, float scaleY, float positionX, float positionY, float mass,
			const char* textureFilepath, bool isStationary, bool affectedByPhysics,
			float maxSpeed);

		void move(Utils::Vector2D movementVector);

        float getMaxSpeed(void);
		void setMaxSpeed(float maxSpeed);
	};
}

#endif
