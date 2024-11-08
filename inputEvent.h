#pragma once

#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include "event.h"
#include "GameObject.h"
#include "input.h"


namespace Events {

	class InputEvent : public virtual Event {
	private:
		// InputHandler value containing inputByte used for reading movements
		InputHandler m_inputHandler;
		// Movement vector that defines the directional inputs being 
		Utils::Vector2D m_movementVector;
	public:

		InputEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, InputHandler inputHandler, Utils::Vector2D movementVector);

		void onEvent() const override;
	};
}



#endif
