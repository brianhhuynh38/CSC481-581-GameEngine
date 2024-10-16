#include <SDL.h>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

#include "vector2D.h"


namespace Utils {

	/**
	* Vector2D default constructor
	*/
	Vector2D::Vector2D(void) {
		this->x = 0.0;
		this->y = 0.0;
	}

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
	 * Adds the given vector to this vector and returns the result.
	 * @param other Vector2D
	 * @returns The result of the addition between the two Vectors
	 */
	Vector2D Vector2D::add(Vector2D other) {
		return Vector2D(this->x + other.x, this->y + other.y);
	}

	/**
	 * Adds the given vector coordinates to this vector and returns the result.
	 * @param x coordinate to add
	 * @param y coordinate to add
	 * @returns The result of the addition between the two Vectors
	 */
	Vector2D Vector2D::add(float x, float y) {
		return Vector2D(this->x + x, this->y + y);
	}

	/**
	 * Subtracts the given vector from this vector and returns the result.
	 * @param other Vector2D
	 * @returns The result of the subtraction between the two Vectors
	 */
	Vector2D Vector2D::subtract(Vector2D other) {
		return Vector2D(this->x - other.x, this->y - other.y);
	}

	/**
	 * Subtracts the given vector coordinates from this vector and returns the result.
	 * @param x coordinate to subtract
	 * @param y coordinate to subtract
	 * @returns The result of the subtraction between the two Vectors
	 */
	Vector2D Vector2D::subtract(float x, float y) {
		return Vector2D(this->x - x, this->y - y);
	}

	/**
	* Multiplies the vector by a constant and returns the result
	* @param constant The constant multiplying the Vector
	* @returns The result of the constant multiplication
	*/
	Vector2D Vector2D::multConst(float constant) {
		return Vector2D(this->x *constant, this->y * constant);
	}

	/**
	* Divides the vector by a constant and returns the result
	* @param constant The constant dividing the vector
	* @returns The result of the constant division
	*/
	Vector2D Vector2D::divideConst(float constant) {
		return Vector2D(this->x / constant, this->y / constant);
	}

	/**
	* Calculates and returns the magnitude of the vector
	* @returns The magnitude of the vector
	*/
	float Vector2D::getMagnitude(void) {
		return sqrt((pow(x, 2) + pow(y, 2)));
	}

	/**
	* Calculates the unit vector for this vector, then returns it
	* @returns The normalized vector
	*/
	Vector2D Vector2D::normalizeVector(void) {
		return this->divideConst(getMagnitude());
	}

	std::string Vector2D::toString() {
		// Create stringstream to stringify vector
		std::stringstream ss;
		// Create string in format X,Y
		ss << x << "," << y << "\n";
		return ss.str();
	}

	Vector2D Vector2D::fromString(std::stringstream& ss) {
		std::string line, xStr, yStr;

		// Try to read the entire line first
		if (!std::getline(ss, line)) {
			throw std::invalid_argument("Invalid input: could not read the line.");
		}

		// Debug: Output the line read from the stringstream
		//std::cout << "Line read: " << line << "\n";

		// Create a stringstream from the line for further parsing
		std::stringstream lineStream(line);

		// Extract the x and y components, separated by a comma
		if (!std::getline(lineStream, xStr, ',')) {
			throw std::invalid_argument("Invalid input: could not parse x value.");
		}

		if (!std::getline(lineStream, yStr)) {
			throw std::invalid_argument("Invalid input: could not parse y value.");
		}

		// Debug: Output the extracted x and y strings
		//std::cout << "Extracted xStr: " << xStr << ", yStr: " << yStr << "\n";

		// Convert the strings to floats
		try {
			float x = std::stof(xStr);
			float y = std::stof(yStr);

			// Debug: Output the parsed float values
			//std::cout << "Parsed x: " << x << ", y: " << y << "\n";

			return Vector2D(x, y);
		}
		catch (const std::exception& e) {
			throw std::invalid_argument("Invalid input: could not convert to float.");
		}
	}
}
