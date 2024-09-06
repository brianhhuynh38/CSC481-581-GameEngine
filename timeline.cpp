#include <SDL_timer.h>
#include <iostream>

#include "timeline.h"

Timeline::Timeline() {
	m_currentTicks = SDL_GetTicks64();
	m_previousTicks = 0;
	m_deltaTime = 0;
}

void Timeline::updateTime() {
	m_currentTicks = SDL_GetTicks64();
	m_deltaTime = (double)((m_currentTicks - m_previousTicks)*1000 / (double)SDL_GetPerformanceFrequency());
	m_previousTicks = m_currentTicks;
	//std::cout << "dTime: " << m_deltaTime << "ms\n";
	
}

double Timeline::getDeltaTime() {
	return m_deltaTime;
}
