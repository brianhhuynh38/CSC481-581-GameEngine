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
		// TODO: Populate
	}

	void applyGravity(double deltaTimeInSecs, float mass, Utils::Vector2D* acceleration) {
		// TODO: Populate
	}

}
