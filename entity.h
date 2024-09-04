#include "vector2D.h"
#include <SDL.h>

#pragma once
#ifndef ENTITY_H
#define ENTITY_H

namespace Entities {

	class Entity {
	protected:
		Utils::Vector2D *m_scale;
		Utils::Vector2D *m_position;
		Utils::Vector2D *m_velocity;
		Utils::Vector2D *m_acceleration;
		/** Weight of entity (changes how gravity affects it) */
		float m_mass;
		/** Texture to use for this entity */
		SDL_Texture *m_texture;
		/** Rectangle collider to use for this entity */
		std::list<SDL_Rect> m_colliders;
		/** Whether or not this entity is stationary */
		bool m_isStationary;
		/** Whether or not this entity is affected by physics (gravity) */
		bool m_affectedByPhysics;
		/** Whether or not the entity should be visably rendered on screen (lets you hide entities)*/
		bool m_isVisible;
	public:

		Entity();

		/**
		* Constructs an entity and initializes all pointer fields.
		*
		* @param scaleX X component of scaling multipliers for rendering
		* @param scaleY Y component of scaling multipliers for rendering
		* @param position The coordinates where the Entity is located
		* @param textureFilepath The filepath to where the texture is located
		* @param isStationary Whether the object should move
		* @param affectedByPhysics Whether the object is affectedByPhysics
		*/
		Entity(float scaleX, float scaleY, float positionX, float positionY, float mass,
			const char* textureFilepath, bool isStationary, bool affectedByPhysics);

		Utils::Vector2D *getScale(void);
		void setScale(float scaleX, float scaleY);

		Utils::Vector2D *getPosition(void);
		void updatePosition(Utils::Vector2D position);
		void setPosition(float positionX, float positionY);

		Utils::Vector2D *getVelocity(void);
		void updateVelocity(Utils::Vector2D velocity);
		void setVelocity(float velocityX, float velocityY);

		Utils::Vector2D *getAcceleration(void);
		void updateAcceleration(Utils::Vector2D acceleration);
		void setAcceleration(float accelerationX, float accelerationY);

		float getMass();
		void setMass(float mass);

		SDL_Texture *getTexture(void);
		void setTexture(char* textureFilepath);

		std::list<SDL_Rect> getColliders(void);
		void setColliders(std::list<SDL_Rect> colliders);
		void addCollider(int x, int y, int w, int h);

		void destroy(void);

	};
}

#endif
