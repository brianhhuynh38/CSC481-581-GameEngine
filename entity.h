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

		/**
		* Constructs an entity and initializes all pointer fields.
		*
		* @param scaleX X component of scaling multipliers for rendering
		* @param scaleY Y component of scaling multipliers for rendering
		* @param position The coordinates where the Entity is located
		* @param textureFilepath The filepath to where the texture is located
		* @param isStationary Whether the object should move
		* @param isPlayer Whether the object is a player-controlled object
		*/
		Entity(float scaleX, float scaleY, float positionX, float positionY,
			const char* textureFilepath, bool isStationary, bool isPlayer);

		Utils::Vector2D *getScale(void);
		void setScale(float scaleX, float scaleY);

		Utils::Vector2D *getPosition(void);
		void setPosition(float positionX, float positionY);

		Utils::Vector2D *getVelocity(void);
		void setVelocity(float velocityX, float velocityY);

		Utils::Vector2D *getAcceleration(void);
		void setAcceleration(float accelerationX, float accelerationY);

		SDL_Texture *getTexture(void);
		void setTexture(char* textureFilepath);

		void destroy(void);
	};
}

#endif
