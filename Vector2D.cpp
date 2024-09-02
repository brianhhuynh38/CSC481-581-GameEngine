#include <SDL.h>
#include <cmath>

#include "vector2D.h"


namespace Utils {

	Vector2D::Vector2D(float x, float y) {
		this->x = x;
		this->y = y;
	}

	void Vector2D::add(Vector2D other) {
		this->x += other.x;
		this->y += other.y;
	}

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
