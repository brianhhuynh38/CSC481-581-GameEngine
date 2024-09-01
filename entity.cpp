#include <SDL.h>

#include "structs.h"
#include "draw.h"

class Entity {
public:

	/**
	* Default entity constructor
	*/
	Entity() {
		size.x = 1;
		size.y = 1;
		position.x = 0;
		position.y = 0;
		velocity.x = 0;
		velocity.y = 0;
		texture = loadTexture((char*)"assets/gfx/player.png");
	}

	/**
	* Custom entity constructor
	* @parm s size
	* @param p positon
	* @param l layer
	* @param v vector
	* @param t texture
	*/ 
	Entity(Vector s, Vector p, int l, Vector v, SDL_Texture* t) {
		size.x = s.x;
		size.y = s.y;
		position.x = p.x;
		position.y = p.y;
		layer = l;
		velocity.x = v.x;
		velocity.y = v.y;
		texture = t;
	}
	
	// Getter/setter for entity size
	Vector getSize() {
		return size;
	}
	void setSize(Vector s) {
		size = s;
	}

	// Getter/setter for entity position
	Vector getPosition(void) {
		return position;
	}
	void setPosition(Vector p) {
		position = p;
	}

	// Getter/setter for entity layer
	int getLayer(void) {
		return layer;
	}
	void setLayer(int l) {
		layer = l;
	}

	// Getter/setter for entity velocity
	Vector getVelocity(void) {
		return velocity;
	}
	void setVelocity(Vector v) {
		velocity = v;
	}

	// Getter/setter for entity texture
	SDL_Texture* getTexture(void) {
		return texture;
	}
	void setTexture(SDL_Texture* t) {
		texture = t;
	}

	// load entity
	void LoadEntity(char* filepath) {

	}
private:
	Vector size;
	Vector position;
	int layer = 0; // higher layer = render on top
	Vector velocity;
	SDL_Texture* texture;
};