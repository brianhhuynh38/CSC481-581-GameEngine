#include "recorder.h"

#include "global.h"

/**
* Recorder constructor
*/
Recorder::Recorder() {
    // Intitalize all data structures
    m_eventHistory = std::queue<Events::Event*>();
    m_startPositions = std::map<int, Utils::Vector2D>();
    m_tempSnapshotPositions = std::map<int, Utils::Vector2D>();
    // Default starting values
    m_startingTimeStamp = -1;
    m_endingTimeStamp = -1;
}

void Recorder::enableRecording() {
    if(!isRecording && !startPlayback) {
        isRecording = true;
    }
}

void Recorder::disableRecording() {
    if(isRecording) {
        isRecording = false;
    }
}

void Recorder::tryRecordEvent(Events::Event* event) {
    if (isRecording) {
        m_eventHistory.push(event);
    }
}

void Recorder::tryDispatchRecording(int64_t timeStamp) {
    if (startPlayback && !isRecording) {
        while (!m_eventHistory.empty() && m_eventHistory.front()->getTimeStampPriority() <= timeStamp - m_startPlaybackTimeOffset) {
            // Get event at the front
            Events::Event* e = m_eventHistory.front();
            // Run the onEvent, then delete
            e->onEvent();
            //delete e;
            // Remove the Event from the queue
            m_eventHistory.pop();
        }
    }
}
