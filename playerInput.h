#pragma once

#include "component.h"
#include "input.h"
#include "vector2D.h"
#include "timeline.h"

#include <SDL.h>

#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H

namespace Component {

	class PlayerInputPlatformer : public virtual Component {
	private:
		float maxMovementSpeed;
		Utils::Vector2D jumpVector;
		InputHandler* m_inputHandler;
	public:
		void update() override {
			// TODO: Update physics vectors
		}
	};

}

#endif
