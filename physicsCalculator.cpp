#include "physicsCalculator.h"

#include "vector2D.h"

#include <iostream>

namespace PhysCalc {

	// Gravity used for calculating physics in the game engine (default 9.81)
	float m_gravity = 9.81f;

	/**
	* Sets the gravity value used for calculations
	*/
	void setGravity(float gravity) {
		m_gravity = gravity;
	}

	/**
	* Updates the vectors sent in as parameters
	*/
	void updatePhysicsVectors(double deltaTimeInSecs, Utils::Vector2D* position,
		Utils::Vector2D* velocity, Utils::Vector2D* acceleration) {
		// Update velocity using acceleration
		*velocity = velocity->add(acceleration->multConst(deltaTimeInSecs));

		// Update position using velocity
		*position = position->add(velocity->multConst(deltaTimeInSecs));

		//std::cout << "Velocity: " << velocity->toString() << "Position: " << position->toString() << "\n";
	}

	void applyGravity(double deltaTimeInSecs, float mass, Utils::Vector2D* acceleration) {
		Utils::Vector2D gravityForce(0, m_gravity * mass);

		*acceleration = Utils::Vector2D(0, m_gravity * mass).multConst(deltaTimeInSecs);
		//*acceleration = acceleration->add(Utils::Vector2D(0, m_gravity * mass).multConst(deltaTimeInSecs));
	}
}
