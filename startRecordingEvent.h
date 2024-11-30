#pragma once

#ifndef STARTRECORDINGEVENT_H
#define STARTRECORDINGEVENT_H

#include "event.h"

#include <vector>

namespace Events {

	class StartRecordingEvent : public virtual Event {
	public:

		StartRecordingEvent(int64_t timeStampPriority, int priority);

		void onEvent() override;
	};

}

#endif
