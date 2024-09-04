#include "vector2D.h"
#include "entity.h"

#pragma once

class Physics {
private:
	float m_gravity;
public:
	Physics(float gravity);

	void updateEntityPhysicsVectors(Entities::Entity entity);

	void applyGravity(Entities::Entity entity);
};