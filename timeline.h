#pragma once

class Timeline {
private:
	//std::mutex lock; // TODO add this later when this is multithreaded
	bool m_paused;
	bool m_reversed;
	int64_t m_tic;
	int64_t m_startingTime;
	int64_t m_currentTime;
	int64_t m_deltaTime;
	float m_timeScale;
	Timeline* m_timeline;
public:

	/**
	* Default constructor for the timeline that sets tic to a default value of 60
	*/
	Timeline();

	Timeline(int64_t tic);

	void updateTime();

	int64_t getDeltaTime();

	int64_t getTime();

	void pause();

	void unpause();

	void reverse();

	void unreverse();

	void changeTic(_int64 tic);

	void changeTimeScale(float timeScale);
};
