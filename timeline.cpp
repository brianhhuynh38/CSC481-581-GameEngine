#include <SDL_timer.h>
#include <iostream>
#include <mutex>
#include <chrono>

#include "timeline.h"

Timeline::Timeline() {
	// TODO: Update to use chronos clock to
	m_currentTime = 0; // Temp
	m_startingTime = 0;
	m_deltaTime = 0;
}

void Timeline::updateTime() {
	// TODO: Make new update time that fits the new chronos implementation


	//m_currentTime = SDL_GetTicks64();
	// No longer applicable
	//m_deltaTime = (double)((m_currentTime - m_startingTime)*1000 / (double)SDL_GetPerformanceFrequency());
	//std::cout << "dTime: " << m_deltaTime << "ms\n";
	
}

double Timeline::getDeltaTime() {
	return m_deltaTime;
}
