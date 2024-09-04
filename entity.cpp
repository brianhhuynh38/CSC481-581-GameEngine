#include <SDL.h>
#include <iostream>
#include <list>

#include "entity.h"
#include "vector2D.h"
#include "draw.h"


namespace Entities {

	/**
	* Default constructor that sets all values to 0
	*/
	Entity::Entity() {
		m_scale = new Utils::Vector2D(1.0, 1.0);
		m_position = new Utils::Vector2D(0.0, 0.0);
		m_velocity = new Utils::Vector2D(0.0, 0.0);
		m_acceleration = new Utils::Vector2D(0.0, 0.0);
		m_mass = 5;
		m_texture = nullptr;
		m_isStationary = false;
		m_affectedByPhysics = true;
	}

	/**
	* Constructs an entity and initializes all pointer fields.
	* 
	* @param scaleX X component of scaling multipliers for rendering
	* @param scaleY Y component of scaling multipliers for rendering
	* @param position The coordinates where the Entity is located
	* @param textureFilepath The filepath to where the texture is located
	* @param isStationary Whether the object should move
	* @param affectedByPhysics Whether the object is affected by physics
	*/
	Entity::Entity(float scaleX, float scaleY, float positionX, float positionY, float mass,
		const char* textureFilepath, bool isStationary, bool affectedByPhysics) {
		m_scale = new Utils::Vector2D(scaleX, scaleY);
		m_position = new Utils::Vector2D(positionX, positionY);
		m_velocity = new Utils::Vector2D(0.0, 0.0);
		m_acceleration = new Utils::Vector2D(0.0, 0.0);
		m_mass = mass;
		m_texture = Render::loadTexture(textureFilepath);
		m_isStationary = isStationary;
		m_affectedByPhysics = affectedByPhysics;
	}
	/**
	* Returns the scale ratio of the entity
	* @returns The scale ratio of the entity as a 2D vector
	*/
	Utils::Vector2D *Entity::getScale(void) {
		return m_scale;
	}

	/**
	* Sets new values to the scale ratio
	* @param scaleX The new x value of the scale to set
	* @param scaleY The new y value of the scale to set
	*/
	void Entity::setScale(float scaleX, float scaleY) {
		m_scale = new Utils::Vector2D(scaleX, scaleY);
	}

	/**
	* Returns the position of the entity
	* @returns The position of the entity as a 2D vector
	*/
	Utils::Vector2D *Entity::getPosition(void) {
		return m_position;
	}

	/**
	* Adds the parameter position vector to the Entity's current position vector
	* @param position: The position to add to the current position
	*/
	void Entity::updatePosition(Utils::Vector2D position) {
		m_position->add(position);
	}

	/**
	* Sets new values to the position ratio
	* @param positionX The new x value of the position to set
	* @param positionY The new y value of the position to set
	*/
	void Entity::setPosition(float positionX, float positionY) {
		m_position = new Utils::Vector2D(positionX, positionY);
	}

	/**
	* Returns the velocity of the entity
	* @returns The velocity of the entity as a 2D vector
	*/
	Utils::Vector2D *Entity::getVelocity(void) {
		return m_velocity;
	}

	/**
	* Adds the parameter velocity vector to the Entity's current velocity vector
	* @param velocity: The velocity to add to the current velocity
	*/
	void Entity::updateVelocity(Utils::Vector2D velocity) {
		m_velocity->add(velocity);
	}

	/**
	* Sets new values to the velocity ratio
	* @param velocityX The new x value of the velocity to set
	* @param velocityY The new y value of the velocity to set
	*/
	void Entity::setVelocity(float velocityX, float velocityY) {
		m_velocity = new Utils::Vector2D(velocityX, velocityY);
	}

	/**
	* Returns the acceleration of the entity
	* @returns The acceleration of the entity as a 2D vector
	*/
	Utils::Vector2D *Entity::getAcceleration(void) {
		return m_acceleration;
	}

	/**
	* Adds the parameter acceleration vector to the Entity's current acceleration vector
	* @param acceleration: The acceleration to add to the current acceleration
	*/
	void Entity::updateAcceleration(Utils::Vector2D acceleration) {
		m_acceleration->add(acceleration);
	}

	/**
	* Sets new values to the acceleration ratio
	* @param accelerationX The new x value of the acceleration to set
	* @param accelerationY The new y value of the acceleration to set
	*/
	void Entity::setAcceleration(float accelerationX, float accelerationY) {
		m_acceleration = new Utils::Vector2D(accelerationX, accelerationY);
	}

	/**
	* Returns the mass of the object
	* @returns the Mass of the object
	*/
	float Entity::getMass() {
		return m_mass;
	}

	/**
	* Sets the mass of the object given a value
	* @param mass The mass of the object to be set
	*/
	void Entity::setMass(float mass) {
		m_mass = mass;
	}

	/**
	* Returns the texture of the entity
	* @returns The texture of the entity as a 2D vector
	*/
	SDL_Texture *Entity::getTexture(void) {
		return m_texture;
	}

	/**
	* Sets the texture by trying to load the file at the given filepath
	* @param textureFilepath The location of the texture to load
	*/
	void Entity::setTexture(char* textureFilepath) {
		try {
			m_texture = Render::loadTexture(textureFilepath);
		}
		catch (std::exception& e) {
			std::cerr << "Could not load texture at: " << textureFilepath;
		}
	}
	
	/**
	* Returns the collider of the entity
	* @returns The collider of the entity as SDL_Rect
	*/
	std::list<SDL_Rect> Entity::getColliders(void) {
		return m_colliders;
	}

	/**
	* Sets the colliders by giving a list of colliders
	* @param colliders List of SDL_Rect colliders
	*/
	void Entity::setColliders(std::list<SDL_Rect> colliders) {
		try {
			m_colliders = colliders;
		}
		catch (std::exception& e) {
			std::cerr << "ERROR: Could not set colliders: " << e.what();
		}
	}

	/**
	* Adds a collider to the list of colliders by giving the coordinates and size
	* @param x: x position coordinate
	* @param y: y position coordinate
	* @param w: width of rect
	* @param h: height of rect
	*/
	void Entity::addCollider(int x, int y, int w, int h) {
		SDL_Rect collider = SDL_Rect();
		collider.x = x;
		collider.y = y;
		collider.w = w;
		collider.h = h;
		m_colliders.emplace_back(collider);
	}

	/**
	* Deletes memory allocated via "new" for each of the fields
	*/
	void Entity::destroy(void) {
		delete m_scale;
		delete m_position;
		delete m_velocity;
		delete m_acceleration;
		SDL_DestroyTexture(m_texture);
	}
}
