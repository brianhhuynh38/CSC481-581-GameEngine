#include "recorder.h"
#include "gameObjectManager.h"
#include "global.h"

/**
* Recorder constructor
*/
Recorder::Recorder(GameObjectManager *goManager) {
    // Intitalize all data structures
    m_eventHistory = std::queue<Events::Event*>();
    m_startPositions;
    
    // Default starting values
    m_startingTimeStamp = -1;
    m_endingTimeStamp = -1;
    m_startPlaybackTimeOffset = -1;
    m_goManager = goManager;
}

void Recorder::tryRecordEvent(Events::Event* event) {
    if (isRecording) {
        // If recording is just starting, then record all current initial positions of GameObjects for replay
        if (m_eventHistory.empty()) {
            m_goManager->serialize(m_startPositions);
        }
        // Push Event into the EventHistory fo later replay
        m_eventHistory.push(event);
    }
}

void Recorder::tryDispatchRecording(int64_t timeStamp) {
    if (startPlayback && !isRecording) {
        // Store current GameObject values in a a serialized json string
        std::string goInfo;
        m_goManager->serialize(goInfo);

        // Load in starting values of GameObjects from serialized string taken from StartRecording
        m_goManager->deserialize(m_startPositions, 2);

        // Loops through all stored events and executes them as they were before
        while (!m_eventHistory.empty() && m_eventHistory.front()->getTimeStampPriority() <= timeStamp - m_startPlaybackTimeOffset) {
            // Get event at the front
            Events::Event* e = m_eventHistory.front();
            // Run the onEvent, then delete
            e->onEvent();
            //delete e;
            // Remove the Event from the queue
            m_eventHistory.pop();
        }

        // Once all Events have been dispatched from EventHistory, disable recording mode and return all
        // GameObjects to their positions from before the recording
        if (m_eventHistory.empty()) {
            startPlayback = false;
            isRecording = false;
            m_goManager->deserialize(goInfo, 2);
        }
    }
}
