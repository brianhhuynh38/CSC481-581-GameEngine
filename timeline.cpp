#include <iostream>
#include <mutex>
#include <chrono>

#include "timeline.h"

Timeline::Timeline() {
	// TODO: Update to use chronos clock to
	
	int64_t temp = std::chrono::steady_clock::now().time_since_epoch().count();
	std::chrono::microseconds; // todo: time to use
	m_startingTime = 0; //temp
	m_currentTime = m_startingTime;
	m_deltaTime = 0.0f;
	m_paused = false;
	m_reversed = false;
	m_tic = 60; // Default tic value
	m_timeScale = 1.0f;
}

Timeline::Timeline(int64_t tic) {
	Timeline();
	m_tic = tic;
}

void Timeline::updateTime() {
	if (!m_paused) {
		m_deltaTime = (m_currentTime - m_startingTime) / m_tic; //temp body
	}
}

double Timeline::getDeltaTime() {
	return m_deltaTime;
}

int64_t Timeline::getTime() {
	return m_currentTime;
}

void Timeline::pause() {
	m_paused = true;
}

void Timeline::unpause() {
	m_paused = false;
}

void Timeline::reverse() {
	m_reversed = true;
}

void Timeline::unreverse() {
	m_reversed = false;
}

void Timeline::changeTic(int64_t tic) {
	m_tic = tic;
}

void Timeline::changeTimeScale(float timeScale) {
	m_timeScale = timeScale;
}
