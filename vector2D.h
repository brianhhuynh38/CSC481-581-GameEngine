#pragma once
#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <string>

namespace Utils {

	/**
	* A 2D vector class that contains basic functions for Vector calculations
	*/
	class Vector2D {
	public:
		float x;
		float y;
		
		Vector2D(void);

		Vector2D(float x, float y);

		Vector2D add(Vector2D other);

		Vector2D add(float x, float y);

		Vector2D multConst(float constant);

		Vector2D divideConst(float constant);

		float getMagnitude(void);

		Vector2D normalizeVector(void);

		std::string toString();
	};
}

#endif