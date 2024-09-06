#pragma once

class Timeline {
private:
	_int64 m_previousTicks;
	_int64 m_currentTicks;
	double m_deltaTime;
public:
	Timeline();

	void updateTime();

	double getDeltaTime();
};
