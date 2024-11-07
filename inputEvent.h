#pragma once

#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include "event.h"

namespace Events {

	class InputEvent : public virtual Event {

		InputEvent(std::vector<GameObject*> goRef, int64_t timeStampPriority, int priority);

		void onEvent() const override;
	};
}



#endif
