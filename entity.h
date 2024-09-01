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
	* @param v vector
	* @param t texture
	*/
	Entity(Vector s, Vector p, int l, Vector v, SDL_Texture* t);

    Vector getSize(void);
    void setSize(Vector s);

	Vector getPosition(void);
    void setPosition(Vector p);

	int getLayer(void);
	void setLayer(int l);

	Vector getVelocity(void);
    void setVelocity(Vector v);

    SDL_Texture* getTexture(void);
    void setTexture(SDL_Texture* t);

	void LoadEntity(char* filepath);
private:
	Vector size;
	Vector position;
	int layer = 0; // higher layer = render on top
	Vector velocity;
	SDL_Texture* texture;
};


#endif