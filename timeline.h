#pragma once

class Timeline {
private:
	_int64 m_previousTicks;
	unsigned int m_deltaTime;
public:
	Timeline();

	void updateTime();

	unsigned int getDeltaTime();
};
