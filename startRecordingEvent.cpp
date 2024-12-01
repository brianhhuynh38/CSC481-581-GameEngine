#include "startRecordingEvent.h"

#include "vector2D.h"
#include "transform.h"
#include "playerGO.h"
#include "boundaryZone.h"
#include "global.h"

#include <iostream>

namespace Events {

	StartRecordingEvent::StartRecordingEvent(int64_t timeStampPriority, int priority) {
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	void StartRecordingEvent::onEvent() {
		// Start recording the events that are called after this timestamp into the queue
		if (!startPlayback) {
			std::cout << "Recording started...\n";
			isRecording = true;
			recorder->setStartingTimeStamp(m_timeStampPriority);
		}
	}
}
