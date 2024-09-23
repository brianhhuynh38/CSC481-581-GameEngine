#include <iostream>
#include <mutex>
#include <chrono>

#include "timeline.h"

Timeline::Timeline() {
	// Find the starting time value using high resolution clock
	auto chronoEpoch = std::chrono::high_resolution_clock::now().time_since_epoch();
	m_startingTime = std::chrono::duration_cast<std::chrono::microseconds>(chronoEpoch).count();
	// Set current and previous time to same value as starting time to start with
	m_currentTime = m_startingTime;
	m_prevTime = m_startingTime;
	m_deltaTime = m_currentTime - m_prevTime;
	m_timePaused = 0;
	// Timeline will start unpaused and in normal timeflow
	m_paused = false;
	m_reversed = false;
	// Tic value for main Timeline
	m_tic = 62;
	// Set anchor to NULL for default main Timeline
	m_anchor = NULL;
	// Timeline will move at normal speed (1x speed) 
	m_timeScale = 1.0f;
}

Timeline::Timeline(Timeline *anchor, int64_t tic) {
	// Calls the default constructor
	Timeline();
	// Sets information related to the anchor Timeline
	m_tic = tic;
	m_anchor = anchor;
	m_startingTime = m_anchor->getTime();
	m_currentTime = m_startingTime;
	m_prevTime = m_startingTime;
	m_deltaTime = m_currentTime - m_prevTime;
}

void Timeline::updateTime() {
	// Update prevTime to match the previous iteration
	m_prevTime = m_currentTime;
	// Updates the currentTime
	if (!m_anchor) { // If non-anchored timeline, get currentTime from chrono
		auto chronoEpoch = std::chrono::high_resolution_clock::now().time_since_epoch();
		m_currentTime = std::chrono::duration_cast<std::chrono::microseconds>(chronoEpoch).count();
	}
	else { // If anchored timeline, get currentTime from anchor
		m_currentTime = m_anchor->getTime();
	}
	// Calculate delta time, is 0 if paused
	if (m_paused) {
		m_deltaTime = 0;
		m_timePaused += m_currentTime - m_prevTime;
	}
	else {
		m_deltaTime = m_currentTime - m_prevTime;
	}
	// If reversed, make deltaTime negative
	m_deltaTime *= m_reversed ? -1 : 1;
	// Apply timescaling
	m_deltaTime *= m_timeScale;
}

double Timeline::getDeltaTime() {
	return m_deltaTime;
}

int64_t Timeline::getTime() {
	return m_anchor ?
		((m_currentTime - m_startingTime) / m_tic) - m_timePaused : // If anchored Timeline, subtract timePaused after tic
		(m_currentTime - m_startingTime - m_timePaused) / m_tic // If main Timeline, subtract timePaused before tic
	;
}

void Timeline::pause() {
	m_paused = true;
}

void Timeline::unpause() {
	m_paused = false;
}

bool Timeline::isPaused() {
	return m_paused;
}

void Timeline::reverse() {
	m_reversed = true;
}

void Timeline::unreverse() {
	m_reversed = false;
}

bool Timeline::isReversed() {
	return m_reversed;
}

void Timeline::changeTic(int64_t tic) {
	m_tic = tic;
}

void Timeline::changeTimeScale(float timeScale) {
	m_timeScale = timeScale;
}
