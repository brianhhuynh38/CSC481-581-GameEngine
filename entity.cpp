#include <SDL.h>
#include <iostream>

#include "entity.h"
#include "structs.h"
#include "draw.h"

namespace Entities {

	Entity::Entity(float scaleX, float scaleY, SDL_Point position, char* textureFilepath, bool isStationary, bool isPlayer) {
		*m_scaleX = scaleX;
		*m_scaleY = scaleY;
		*m_position = position;
		*m_velocity = SDL_Point() = { 0, 0 };
		*m_acceleration = SDL_Point() = { 0, 0 };
		m_texture = Render::loadTexture(textureFilepath);
		m_isStationary = isStationary;
		m_isPlayer = isPlayer;
	}

	SDL_Point* Entity::getPosition(void) {
		return m_position;
	}

	SDL_Point* Entity::getVelocity(void) {
		return m_velocity;
	}

	SDL_Point* Entity::getAcceleration(void) {
		return m_acceleration;
	}

	SDL_Texture* Entity::getTexture(void) {
		return m_texture;
	}


	void Entity::setTexture(char* textureFilepath) {
		m_texture = Render::loadTexture(textureFilepath);
	}

	void destroy(void) {

	}
}
