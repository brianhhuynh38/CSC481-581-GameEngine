#include "draw.h"
#include "structs.h"
#pragma once
#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
	/**
	* Default entity constructor
	*/
    Entity();

	/**
	* Custom entity constructor
	* @parm s size
	* @param p positon
	* @param l layer
	* @param v velocity
	* @param t texture
	*/
	Entity(Vector s, Vector p, float l, Vector v, SDL_Texture* t);

	/**
	* Gets the entities xy-size values.
	* @return size vector of entity
	*/
    Vector getSize(void);
	/**
	* Sets the entities xy-size values to the given vector.
	* @param s size vector to set
	*/
    void setSize(Vector s);

	/**
	* Gets the entities xy-position value
	* @return position vector of entity
	*/
	Vector getPosition(void);
	/**
	* Sets the entities xy-position values to the given vector.
	* @param p position vector to set
	*/
    void setPosition(Vector p);

	/**
	* Gets the entities layer value.
	* @return layer int of entity
	*/
	float getLayer(void);
	/**
	* Sets the entities layer value to the given int.
	* @param l layer int to set
	*/
	void setLayer(float l);

	/**
	* Gets the entities xy-velocity values to the given vector.
	* @return velocity vector of entity
	*/
	Vector getVelocity(void);
	/**
	* Sets the entities xy-velocity values to the given vector.
	* @param v velocity vector to set
	*/
    void setVelocity(Vector v);

	/**
	* Gets the entities texture value.
	* @return texture of entity
	*/
    SDL_Texture* getTexture(void);
	/**
	* Sets the entities texture value to the given texture.
	* @param t texture to set
	*/
    void setTexture(SDL_Texture* t);

	/**
	* Loads the entity from the given filepath.
	* @param filepath of entity to load
	*/
	void LoadEntity(char* filepath);
private:
	/** Vector size of entity */
	Vector size;
	/** Vector position of entity */
	Vector position;
	/** RenderLayer of entity. (higher layer = render on top) */
	float layer = 0;
	/** Vector velocity of entity */
	Vector velocity;
	/** Texture of entity */
	SDL_Texture* texture;
};


#endif