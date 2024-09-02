#pragma once
#ifndef VECTOR2D_H
#define VECTOR2D_H

namespace Utils {

	/**
	* A 2D vector class that contains basic functions for Vector calculations
	*/
	class Vector2D {
	public:
		float x;
		float y;
		Vector2D(float x, float y);

		void add(Vector2D other);

		void add(float x, float y);

		void multConst(float constant);

		float getMagnitude();
	};
}

#endif