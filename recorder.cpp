#include "recorder.h"
#include "gameObjectManager.h"
#include "global.h"

/**
* Recorder constructor
*/
Recorder::Recorder(GameObjectManager *goManager) {
    // Intitalize all data structures
    m_eventHistory = std::queue<Events::Event*>();
    m_goManager = goManager;
    
    // Default starting values
    m_startingTimeStamp = -1;
    m_startPlaybackTimeOffset = -1;
    m_startPositions = "";
    m_tempPositions = "";
    m_recordingInitialized = false;
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
        try {
            // Create startup information when playback is just starting
            if (!m_recordingInitialized) {
                std::cout << "Preparing playback...\n";

                std::cout << "Saving GameObject information...\n";

                // Store current GameObject values in a a serialized json string
                m_goManager->serialize(m_tempPositions);

                std::cout << "Loading previous GameObject information...\n";

                // Load in starting values of GameObjects from serialized string taken from StartRecording
                m_goManager->deserializeAll(m_startPositions, 2);

                std::cout << "Starting playback...\n";
                m_startPlaybackTimeOffset = timeStamp - m_startingTimeStamp;
                m_recordingInitialized = true;
            }
        }
        catch (std::exception e) {
            std::cout << "Exception: " << e.what() << "\n" << "Recording has failed, exiting...\n";
            return;
        }

        std::cout << "Starting playback loop...\n";

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
            m_goManager->deserializeAll(m_tempPositions, 2);
            std::cout << "Finished Playback.\n";
            m_recordingInitialized = false;
        }
    }
}

void Recorder::setStartingTimeStamp(int64_t timeStamp) {
    m_startingTimeStamp = timeStamp;
}
