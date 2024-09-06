#include <SDL_timer.h>

#include "timeline.h"

Timeline::Timeline() {
	m_previousTicks = SDL_GetTicks64();
	m_deltaTime = 0;
}

void Timeline::updateTime() {
	unsigned int temp = m_deltaTime;
	m_deltaTime = (SDL_GetTicks64() - m_previousTicks) / SDL_GetPerformanceFrequency();
	m_previousTicks = m_deltaTime;
	

}

unsigned int Timeline::getDeltaTime() {
	return m_deltaTime;
}
