#pragma once

#include "component.h"

#ifndef TRANSFORM_H
#define TRANSFORM_H

namespace Component {

	/**
	* The transform is completely necessary for a GameObject, as it stores all relevant information related to 
	* the object's position and existence.
	*/
	class Transform : public virtual Component {
	private:
		/** The current position of the GameObject */
		Utils::Vector2D *m_position;
		/** The reference to the global camera offset value used to keep track of the camera's position */
		Utils::Vector2D *m_cameraOffset;
		/** The size of the GameObject in pixels */
		Utils::Vector2D m_size;
		/** The scale of the object to be multiplied by */
		Utils::Vector2D m_scale;
	public:

		/**
		* Constructs transform with all necessary fields
		*/
		Transform(Utils::Vector2D position, Utils::Vector2D *offsetRef, Utils::Vector2D size, Utils::Vector2D scale) {
			// Set position values
			m_position = new Utils::Vector2D();
			*m_position = position;

			// Set reference to global camera offset value
			m_cameraOffset = offsetRef;

			// Set size and scale
			m_size = size;
			m_scale = scale;
		}

		/**
		* Destructor for the Transform
		*/
		~Transform() override {
			delete m_position;
		}

		void update() override {
			// Not applicable currently
		}

		/**
		 * Returns the position
		 */
		Utils::Vector2D *getPosition(void) {
			return m_position;
		}

		/**
	     * Adds the parameter position vector to the Entity's current position vector
		 * @param position: The position to add to the current position
	     */
		void updatePosition(Utils::Vector2D position) {
			*m_position = m_position->add(position);
		}

		/**
		 * Sets the position using the two float values provided
		 * @param positionX: The x component of the position
		 * @param positionY: The y component of the position
		 */
		void setPosition(float positionX, float positionY) {
			m_position = new Utils::Vector2D(positionX, positionY);
		}

		/**
		 * Returns the height and width
		 * @return m_size
		 */
		Utils::Vector2D getSize(void) {
			return m_size;
		}

		/**
		 * Sets the height and width
		 * @param width to set
		 * @param height to set
		 */
		void setSize(float width, float height) {
			m_size = Utils::Vector2D(width, height);
		}

		/**
		 * Returns the scale ratio of the entity
		 * @returns m_scale the scale ratio of the entity as a 2D vector
		 */
		Utils::Vector2D getScale(void) {
			return m_scale;
		}

		/**
		 * Sets the scale values
		 * @param scaleX
		 * @param scaleY
		 */
		void setScale(float scaleX, float scaleY) {
			m_scale = Utils::Vector2D(scaleX, scaleY);
		}


	};

}

#endif
