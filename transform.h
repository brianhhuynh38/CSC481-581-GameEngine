#pragma once

#include "component.h"
#include "vector2D.h"

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
	};

}

#endif
