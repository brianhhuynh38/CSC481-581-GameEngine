#pragma once

#ifndef INPUTEVENT_H
#define INPUTEVENT_H

#include "event.h"

namespace Events {

	class InputEvent : public virtual Event {
		void onEvent();
	};
}



#endif
