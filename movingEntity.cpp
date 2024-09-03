#include <SDL.h>
#include <cmath>

#include "movingEntity.h"
#include "draw.h"

namespace Entities {
	MovingEntity::MovingEntity(float scaleX, float scaleY, float positionX, float positionY, float mass,
		const char* textureFilepath, bool isStationary, bool affectedByPhysics,
		bool continuous, bool reverse, bool active, int pauseTimer, float speed, float progress) {
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
		m_active = active;
		m_pauseTimer = pauseTimer;
		m_speed = speed;
		m_progress = progress;
	}

	void MovingEntity::move() {
		// TODO: Implement
		*m_position;
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

	bool MovingEntity::isActive() {
		return m_active;
	}

	void MovingEntity::toggleActive(bool active) {
		m_active = active;
	}

	float MovingEntity::getSpeed() {
		return m_speed;
	}

	void MovingEntity::setSpeed(float speed) {
		m_speed = speed;
	}

	float MovingEntity::getProgress() {
		return m_progress;
	}
	
	void MovingEntity::setProgress(float progress) {
		m_progress = progress;
	}

	Utils::Vector2D MovingEntity::getEndPosition() {
		return m_endPosition;
	}

	void MovingEntity::setEndPosition(Utils::Vector2D position) {
		m_endPosition = position;
	}
}
