#include "stopRecordingEvent.h"

#include "vector2D.h"
#include "transform.h"
#include "playerGO.h"
#include "boundaryZone.h"
#include "global.h"

#include <iostream>

namespace Events {

	StopRecordingEvent::StopRecordingEvent(int64_t timeStampPriority, int priority) {
		this->m_timeStampPriority = timeStampPriority;
		this->m_priority = priority;
	}

	void StopRecordingEvent::onEvent() {
		// Stop recording the events that are called after this timestamp into the queue. Should probably create and call the "startPlaybackEvent"
		if (!startPlayback) {
			std::cout << "Recording stopped, please press P in order to playback your recording...\n";
			isRecording = false;
		}
	}
}