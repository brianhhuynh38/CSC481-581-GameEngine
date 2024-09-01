#include "draw.h"
#include "structs.h"
#pragma once
#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
    Entity();
    Entity(Vector s, Vector p, Vector v, SDL_Texture* t);
    Vector getSize();
    void setSize(Vector s);
	Vector getPosition();
    void setPosition(Vector p);
	Vector getVelocity();
    void setVelocity(Vector v);
    SDL_Texture* getTexture();
    void setTexture(SDL_Texture* t);
	void LoadEntity(char* filepath);
private:
	Vector size;
	Vector position;
    int layer; // higher layer = render on top
	Vector velocity;
	SDL_Texture* texture;
};


#endif