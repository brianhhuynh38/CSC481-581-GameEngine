#include <SDL_timer.h>

#include "timeline.h"

Timeline::Timeline() {
	m_previousTicks = SDL_GetTicks64();
	m_deltaTime = 0;
}

void Timeline::updateTime() {
	m_deltaTime = SDL_GetTicks64() - m_previousTicks;
}

unsigned int Timeline::getDeltaTime() {
	return m_deltaTime;
}
