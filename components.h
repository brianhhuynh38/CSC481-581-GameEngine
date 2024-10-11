#pragma once

#include "vector2D.h"

#ifndef COMPONENTS_H
#define COMPONENTS_H

namespace Component {

	/**
	* Generic template class to represent Components of a GameObject
	*/
	template <typename T>
	class Component {
	protected:
		T *m_component;
	public:
		/* Destructor of the component */
		virtual ~Component() { delete m_component; }
		/* Virtual function for getting the component */
		T* getComponent() { return m_component; }

		void setComponent(T value) { *m_component = value }

		virtual void update(T value) = 0;
	};

	/**
	* Class that represents the position of a GameObject
	*/
	class Position : public Component<Utils::Vector2D> {
	public:
		/* Constructor for Position component */
		Position(float x, float y) { m_component = new Utils::Vector2D(x, y); }

		void update(Utils::Vector2D position) {
			*m_component = m_component->add(position);
		}
	};
}

#endif
