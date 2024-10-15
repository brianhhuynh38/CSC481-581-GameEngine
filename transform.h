#pragma once

#include "component.h"

#ifndef TRANSFORM_H
#define TRANSFORM_H

namespace Components {

	/**
	* The transform is completely necessary for a GameObject, as it stores all relevant information related to 
	* the object's position and existence.
	*/
	class Transform : public virtual Component {
	protected:
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
		Transform(Utils::Vector2D position, Utils::Vector2D* offsetRef, Utils::Vector2D size, Utils::Vector2D scale);

		/**
		* Destructor for the Transform
		*/
		~Transform() override;

		/**
		* Updates the transform (currently does nothing)
		*/
		void update() override;

		/**
		 * Returns the position
		 */
		Utils::Vector2D* getPosition(void);

		/**
	     * Adds the parameter position vector to the Entity's current position vector
		 * @param position: The position to add to the current position
	     */
		void updatePosition(Utils::Vector2D position);
		/**
		 * Sets the position using the two float values provided
		 * @param positionX: The x component of the position
		 * @param positionY: The y component of the position
		 */
		void setPosition(float positionX, float positionY);

		/**
		 * Returns the height and width
		 * @return m_size
		 */
		Utils::Vector2D getSize(void);

		/**
		 * Sets the height and width
		 * @param width to set
		 * @param height to set
		 */
		void setSize(float width, float height);

		/**
		 * Returns the scale ratio of the entity
		 * @returns m_scale the scale ratio of the entity as a 2D vector
		 */
		Utils::Vector2D getScale(void);

		/**
		 * Sets the scale values
		 * @param scaleX
		 * @param scaleY
		 */
		void setScale(float scaleX, float scaleY);
	};

}

#endif
