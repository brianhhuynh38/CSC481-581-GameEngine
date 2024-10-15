#pragma once

#ifndef PLAYERINPUT_H
#define PLAYERINPUT_H

namespace Component {

	class PlayerInputPlatformer : public virtual Component {
	private:
		float maxMovementSpeed;
		Utils::Vector2D jumpVector;
		//InputHandler* m_inputHandler;
	public:
		void update() override;
	};

}

#endif
