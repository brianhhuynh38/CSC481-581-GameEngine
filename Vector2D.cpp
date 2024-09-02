#include <SDL.h>
#include <cmath>

#include "vector2D.h"


namespace Utils {
	/**
	 * Vector2D constructor
	 * @param x coordinate
	 * @param y coordinate
	 */
	Vector2D::Vector2D(float x, float y) {
		this->x = x;
		this->y = y;
	}

	/**
	 * Adds the given vector to this vector.
	 * @param other Vector2D
	 */
	void Vector2D::add(Vector2D other) {
		this->x += other.x;
		this->y += other.y;
	}

	/**
	 * Adds the given vector coordinates to this vector.
	 * @param x coordinate to add
	 * @param y coordinate to add
	 */
	void Vector2D::add(float x, float y) {
		this->x += x;
		this->y += y;
	}

	void Vector2D::multConst(float constant) {
		this->x *= x;
		this->y *= y;
	}

	float Vector2D::getMagnitude() {
		return sqrt((pow(x, 2) + pow(y, 2)));
	}
}
