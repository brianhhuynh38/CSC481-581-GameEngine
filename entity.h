#include "vector2D.h"

#pragma once
#ifndef ENTITY_H
#define ENTITY_H

namespace Entities {

	class Entity {
	private:
		Utils::Vector2D *m_scale;
		Utils::Vector2D *m_position;
		Utils::Vector2D *m_velocity;
		Utils::Vector2D *m_acceleration;
		SDL_Texture *m_texture;
		bool m_isStationary;
		bool m_isPlayer;
	public:

		Entity(float scaleX, float scaleY, float positionX, float positionY,
			const char* textureFilepath, bool isStationary, bool isPlayer);

		Utils::Vector2D *getPosition(void);

		Utils::Vector2D *getVelocity(void);

		Utils::Vector2D *getAcceleration(void);

		SDL_Texture *getTexture(void);

		void setTexture(char* textureFilepath);

		void destroy(void);
	};
}

#endif
