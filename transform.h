#pragma once

#include "component.h"

#ifndef TRANSFORM_H
#define TRANSFORM_H

namespace Component {

	class Transform : public virtual Component {
	private:
		Utils::Vector2D m_position;
		Utils::Vector2D m_cameraOffset;
		Utils::Vector2D m_size;
		Utils::Vector2D m_scale;
	public:
		void update() override {
			// TODO: Update camera offset
		}

		/**
		 * Returns the position
		 */
		Utils::Vector2D getPosition(void) {
			return m_position;
		}

		/**
	     * Adds the parameter position vector to the Entity's current position vector
		 * @param position: The position to add to the current position
	     */
		void updatePosition(Utils::Vector2D position) {
			m_position = m_position.add(position);
		}

		/**
		 * Sets the position using the two float values provided
		 * @param positionX: The x component of the position
		 * @param positionY: The y component of the position
		 */
		void setPosition(float positionX, float positionY) {
			m_position = Utils::Vector2D(positionX, positionY);
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
