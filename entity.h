#include "vector2D.h"
#include <SDL.h>
#include <list>

#pragma once
#ifndef ENTITY_H
#define ENTITY_H

namespace Entities {

	/**
	* Class that represents objects and acts as a superclass to subsets of more specified 
	* objects within the game. Can be affected by physics if enabled.
	*/
	class Entity {
	protected:
		/** Unique identifier for each Entity */
		int m_uuid;
		/** The scaling vector for the Entity */
		Utils::Vector2D *m_scale;
		/** The position the Entity is currently at */
		Utils::Vector2D *m_position;
		/** The size of the entity. */
		Utils::Vector2D *m_size;
		/** Weight of entity (changes how gravity affects it) */
		float m_mass;

		/** The velocity the Entity is moving at */
		Utils::Vector2D *m_velocity;
		/** Max velocity */
		float m_velocity_max;
		/** The acceleration at which the Entity is accelerating */
		Utils::Vector2D *m_acceleration;
		/** Max acceleration */
		float m_acceleration_max;

		/** The filepath to the texture to be used */
		const char *m_textureFilepath;
		/** Texture to use for this entity */
		SDL_Texture *m_texture;

		/** Rectangle collider to use for this entity */
		std::list<SDL_Rect> *m_colliders;
		/** Whether or not this entity is stationary */
		bool m_isStationary;
		/** Whether or not this entity is affected by physics (gravity) */
		bool m_affectedByPhysics;
		/** Whether or not the entity should be visably rendered on screen (lets you hide entities)*/
		bool m_isVisible;
	public:

		/**
		 * Default constructor that intializes all values to 0
		 */
		Entity();

		/**
		 * Constructs an entity and initializes all pointer fields.
		 *
		 * @param uuid The unique id for each entity
		 * @param scaleX X component of scaling multipliers for rendering
		 * @param scaleY Y component of scaling multipliers for rendering
		 * @param positionX The X coordinates where the Entity is located
		 * @param positionY The Y coordinates where the Entity is located
		 * @param width The width size of the Entity
		 * @param height The height size of the Entity
		 * @param mass The mass of the Entity
		 * @param textureFilepath The filepath to where the texture is located
		 * @param isStationary Whether the object should move
		 * @param affectedByPhysics Whether the object is affectedByPhysics
		 */
		Entity(int uuid, float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
			const char* textureFilepath, bool isStationary, bool affectedByPhysics);

		/**
		 * Returns the scale
		 */
		Utils::Vector2D *getScale(void);
		void setScale(float scaleX, float scaleY);

		/**
		 * Returns the position
		 */
		Utils::Vector2D *getPosition(void);

		/**
		 * Updates the position by adding the provided position to the current one
		 * 
		 * @param position: The position to add
		 */
		void updatePosition(Utils::Vector2D position);

		/**
		 * Sets the position using the two float values provided
		 * 
		 * @param positionX: The x component of the position
		 * @param positionY: The y component of the position
		 */
		void setPosition(float positionX, float positionY);

		/**
		 * Returns the height and width
		 * @return m_size
		 */
		Utils::Vector2D* getSize(void);

		/**
		 * sets the height and width
		 * @param width to set
		 * @param height to set
		 */
		void setSize(float width, float height);

		/**
		 * Returns the velocity
		 */
		Utils::Vector2D *getVelocity(void);

		/**
		 * Updates the velocity by adding the provided velocity vector to the current one
		 * 
		 * @param velocity: The velocity to be added to the current velocity
		 */
		void updateVelocity(Utils::Vector2D velocity);

		/**
		 * Sets the velocity using the two provided float values
		 */
		void setVelocity(float velocityX, float velocityY);

		/**
		 * Returns the acceleration
		 */
		Utils::Vector2D *getAcceleration(void);

		/**
		 * Updates the acceleration by adding the provided acceleration vector the the current one
		 * 
		 * @param acceleration: The aceleration vector to add
		 */
		void updateAcceleration(Utils::Vector2D acceleration);

		/**
		 * Sets the acceleration to the acceleration vector provided
		 * 
		 * @param accelerationX: The x component of the acceleration
		 * @param accelerationY: The y component of the acceleration
		 */
		void setAcceleration(float accelerationX, float accelerationY);

		/**
		 * Returns the mass
		 */
		float getMass();

		/**
		 * Sets the mass
		 * 
		 * @param mass: The mass to set
		 */
		void setMass(float mass);

		/**
		 * Returns the pointer to the SDL_Texture
		 */
		SDL_Texture *getTexture(void);

		/**
		 * Loads the texture at the given filepath
		 */
		void setTexture(char* textureFilepath);

		/**
		 * Returns the list of colliders
		 */
		std::list<SDL_Rect> *getColliders(void);

		/**
		 * Set a list of colliders 
		 * 
		 * @param colliders: The colliders to set
		 */
		void setColliders(std::list<SDL_Rect> colliders);

		/**
		 * Adds a collider to the list of colliders
		 * 
		 * @param x: The x component of the collider's position
		 * @param y: The y component of the collider's position
		 * @param w: The width of the collider
		 * @param h: The height of the collider
		 */
		void addCollider(int x, int y, int w, int h);

		/**
		 * Returns true if this entity should be affected by physics
		 * @return affectedByPhysics
		 */
		bool getAffectedByPhysics(void);

		/**
		 * Sets value of affectedByPhysics for the entity
		 * @param affectedByPhysics
		 */
		void setAffectedByPhysics(bool affectedByPhysics);

		/**
		* Generates and returns a string containing all entity information
		*/
		std::string toString();

		/**
		 * Deletes objects to free memory allocated to each relevant field
		 */
		void destroy(void);

	};
}

#endif
