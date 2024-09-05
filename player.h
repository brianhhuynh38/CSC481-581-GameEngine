#include "vector2D.h"
#include "entity.h"

#pragma once
#ifndef PLAYER_H
#define PLAYER_H

namespace Entities {

	/**
	 * The player entity that is able to be controlled by the user
	 */
	class Player : public Entity {
	private:
		/** Max speed at which the player moves */
        float m_maxSpeed;
		/** Whether or not the player is on the ground. */
		bool isGrounded = false;
		/** Initial velocity vector to apply when jumping */
		Utils::Vector2D *m_jumpVector;
	public:

		/**
		 * The constructor for Player
		 * 
		 * @param scaleX X component of scaling multipliers for rendering
		 * @param scaleY Y component of scaling multipliers for rendering
		 * @param position The coordinates where the Entity is located
		 * @param size The coordinates where the Entity is located
		 * @param mass The mass of the Entity
		 * @param textureFilepath The filepath to where the texture is located
		 * @param isStationary Whether the object should move
		 * @param affectedByPhysics Whether the object is affectedByPhysics
		 * @param maxSpeed The maximum speed at whihc the player can move
		 */
		Player(float scaleX, float scaleY, float positionX, float positionY, float width, float height, float mass,
			const char* textureFilepath, bool isStationary, bool affectedByPhysics,
			float maxSpeed);

		/**
		 * Moves the player given a movement vector
		 * 
		 * @param movementVector The vector used to move by adding to velocity
		 */
		void move(Utils::Vector2D movementVector);

		/**
		 * Returns the max speed of the entity
		 */
        float getMaxSpeed(void);

		/**
		 * Sets the max speed to the given amount
		 * 
		 * @param maxSpeed The maximum speed to be set
		 */
		void setMaxSpeed(float maxSpeed);

		/**
		 * Returns jumpVector of player
		 * @return jumpVector of player
		 */
		Utils::Vector2D* getJumpVector();

		/**
		 * Sets players jumpVector value to the given
		 * @param jumpVector value
		 */
		void setJumpVector(Utils::Vector2D jVector);

		/**
		 * Returns true if on ground
		 * @return true if on ground
		 */
		bool getIsGrounded();

		/**
		 * sets players isGrounded value to the given
		 * @param grounded value
		 */
		void setIsGrounded(bool grounded);
	};
}

#endif
