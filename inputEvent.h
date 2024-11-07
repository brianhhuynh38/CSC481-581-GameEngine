#pragma once

#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include "event.h"


namespace Events {

	class InputEvent : public virtual Event {
	private:
		int8_t m_inputByte;
	public:

		InputEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority, int8_t inputByte);

		void onEvent() const override;
	};
}



#endif
