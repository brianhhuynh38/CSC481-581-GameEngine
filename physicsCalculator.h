#pragma once

#ifndef PHYSICSCALCULATOR_H
#define PHYSICSCALCULATOR_H

#include "vector2D.h"

namespace PhysCalc {

	void setGravity(float gravity);

	/**
	* Updates the vectors sent in as parameters
	*/
	void updatePhysicsVectors(double deltaTimeInSecs, Utils::Vector2D* position,
		Utils::Vector2D* velocity, Utils::Vector2D* acceleration);

	void applyGravity(double deltaTimeInSecs, float mass, Utils::Vector2D* acceleration);

}

#endif