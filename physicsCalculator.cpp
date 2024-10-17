#include "physicsCalculator.h"

#include "vector2D.h"

namespace PhysCalc {

	float m_gravity;

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
		*position = velocity->multConst(deltaTimeInSecs);
	}

	void applyGravity(double deltaTimeInSecs, float mass, Utils::Vector2D* acceleration) {
		Utils::Vector2D gravityForce(0, m_gravity * mass);

		*acceleration = acceleration->add(Utils::Vector2D(0, m_gravity * mass).multConst(deltaTimeInSecs));
	}

}
