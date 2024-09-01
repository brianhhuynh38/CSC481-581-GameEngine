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
	* @param v velocity
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
	
	/**
	* Gets the entities xy-size values.
	* @return size vector of entity
	*/
	Vector getSize() {
		return size;
	}
	/**
	* Sets the entities xy-size values to the given vector.
	* @param s size vector to set
	*/
	void setSize(Vector s) {
		size = s;
	}

	/**
	* Gets the entities xy-position value
	* @return position vector of entity
	*/
	Vector getPosition(void) {
		return position;
	}
	/**
	* Sets the entities xy-position values to the given vector.
	* @param p position vector to set
	*/
	void setPosition(Vector p) {
		position = p;
	}

	/**
	* Gets the entities layer value.
	* @return layer int of entity
	*/
	int getLayer(void) {
		return layer;
	}
	/**
	* Sets the entities layer value to the given int.
	* @param l layer int to set
	*/
	void setLayer(int l) {
		layer = l;
	}

	/**
	* Gets the entities xy-velocity values to the given vector.
	* @return velocity vector of entity
	*/
	Vector getVelocity(void) {
		return velocity;
	}
	/**
	* Sets the entities xy-velocity values to the given vector.
	* @param v velocity vector to set
	*/
	void setVelocity(Vector v) {
		velocity = v;
	}

	/**
	* Gets the entities texture value.
	* @return texture of entity
	*/
	SDL_Texture* getTexture(void) {
		return texture;
	}
	/**
	* Sets the entities texture value to the given texture.
	* @param t texture to set
	*/
	void setTexture(SDL_Texture* t) {
		texture = t;
	}

	/**
	* Loads the entity from the given filepath.
	* @param filepath of entity to load
	*/
	void LoadEntity(char* filepath) {

	}
private:
	/** Vector size of entity */
	Vector size;
	/** Vector position of entity */
	Vector position;
	/** RenderLayer of entity. (higher layer = render on top) */
	int layer = 0;
	/** Vector velocity of entity */
	Vector velocity;
	/** Texture of entity */
	SDL_Texture* texture;
};