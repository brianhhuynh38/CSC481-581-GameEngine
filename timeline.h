#pragma once

class Timeline {
private:
	std::mutex lock;
	bool m_paused;
	_int64 m_tic;
	_int64 m_startingTime;
	_int64 m_currentTime;
	double m_deltaTime;
	Timeline* timeline;
public:
	Timeline();

	void updateTime();

	double getDeltaTime();

	void pause();

	void unpause();
};
