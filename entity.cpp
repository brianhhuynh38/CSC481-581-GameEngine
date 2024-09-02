#include <SDL.h>
#include <iostream>

#include "entity.h"
#include "vector2D.h"
#include "draw.h"

namespace Entities {

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
	Entity::Entity(float scaleX, float scaleY, float positionX, float positionY, const char* textureFilepath, bool isStationary, bool isPlayer) {
		m_scale = new Utils::Vector2D(scaleX, scaleY);
		m_position = new Utils::Vector2D(positionX, positionY);
		m_velocity = new Utils::Vector2D({ 0.0, 0.0 });
		m_acceleration = new Utils::Vector2D({ 0.0, 0.0 });
		m_texture = Render::loadTexture(textureFilepath);
		m_isStationary = isStationary;
		m_isPlayer = isPlayer;
	}

	Utils::Vector2D *Entity::getScale(void) {
		return m_scale;
	}

	void Entity::setScale(float scaleX, float scaleY) {
		m_scale = new Utils::Vector2D(scaleX, scaleY);
	}

	Utils::Vector2D *Entity::getPosition(void) {
		return m_position;
	}

	void Entity::setPosition(float positionX, float positionY) {
		m_position = new Utils::Vector2D(positionX, positionY);
	}

	Utils::Vector2D *Entity::getVelocity(void) {
		return m_velocity;
	}

	void Entity::setVelocity(float velocityX, float velocityY) {
		m_velocity = new Utils::Vector2D(velocityX, velocityY);
	}

	Utils::Vector2D *Entity::getAcceleration(void) {
		return m_acceleration;
	}

	void Entity::setAcceleration(float accelerationX, float accelerationY) {
		m_acceleration = new Utils::Vector2D(accelerationX, accelerationY);
	}

	SDL_Texture *Entity::getTexture(void) {
		return m_texture;
	}


	void Entity::setTexture(char* textureFilepath) {
		m_texture = Render::loadTexture(textureFilepath);
	}

	void Entity::destroy(void) {
		delete m_scale;
		delete m_position;
		delete m_velocity;
		delete m_acceleration;
		SDL_DestroyTexture(m_texture);
	}
}
