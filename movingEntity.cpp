#include <SDL.h>
#include <cmath>

#include "movingEntity.h"
#include "draw.h"

namespace Entities {
	MovingEntity::MovingEntity(float scaleX, float scaleY, float positionX, float positionY, float mass,
		const char* textureFilepath, bool isStationary, bool affectedByPhysics,
		bool continuous, bool reverse, bool active, int pauseTimer, float speed, float progress) {
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
		m_active = active;
		m_pauseTimer = pauseTimer;
		m_speed = speed;
		m_progress = progress;
	}

	void MovingEntity::move() {
		
	}

	
}
