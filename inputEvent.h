#pragma once

#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include "event.h"
#include "GameObject.h"
#include "input.h"


namespace Events {

	class InputEvent : public virtual Event {
	private:
		int8_t m_inputByte;
	public:

		InputEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, InputHandler inputByte);

		void onEvent() const override;
	};
}



#endif
