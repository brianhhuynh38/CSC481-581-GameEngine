#pragma once

/**
* The Timeline class manages the passage of time for relevant processes within the
* game engine. It can also be used to anchor to other Timelines to fix its passage of
* time to be in relation to the anchor.
* 
* This class was refactored to be a more in-depth Timeline implementation using the
* slides on Time presented in class:
* https://docs.google.com/presentation/d/1EZ9PFmYsufonouzSsVSzEfTW6dvkegJ7/edit#slide=id.p69
* 
*/
class Timeline {
private:
	//std::mutex lock; // TODO add this later when this is multithreaded

	/** Bool that determines whether the timeline is paused */
	bool m_paused;
	/** Denotes that the timeline is reversed */
	bool m_reversed;
	/** The amount of time units of the anchor timeline that counts as one unit in this timeline */
	int64_t m_tic;
	/** The time at which the timeline was created */
	int64_t m_startingTime;
	/** The time at which the previous iteration was */
	int64_t m_prevTime;
	/** The time of the current iteration */
	int64_t m_currentTime;
	/** The amount of time this Timeline has been paused */
	int64_t m_timePaused;
	/** The difference in time values between previous and current time */
	double m_deltaTime;
	/** The multiplier that is able to speed up or slow down the flow of time */
	float m_timeScale;
	/** The anchor Timeline that the current one references if they use the same time */
	Timeline* m_anchor;
public:

	/**
	* Constructor used for the main Timeline that sets to a tic value of 1 and sets a NULL anchor
	*/
	Timeline();

	/**
	* Constructor used for Timelines that are anchored to another one
	* 
	* @param anchor The anchor Timeline that the one being created will reference
	* @param tic The amount of time units from the anchor Timeline that will be used for this one
	*/
	Timeline(Timeline *anchor, int64_t tic);

	/**
	* Updates relevant values that should be changing with each iteration of the Timeline. This
	* is primarily currentTime, prevTime, and deltaTime.
	*/
	void updateTime();

	/**
	* Returns the value for deltaTime: currentTime - prevTime
	* 
	* @returns The current deltaTime value
	*/
	double getDeltaTime();

	/**
	* Returns the current time of this specific Timeline. If the Timeline has an anchor then
	* it will return a modified value of the currentTime from the anchor based on the tic.
	* 
	* @returns The currentTime specific to this Timeline
	*/
	int64_t getTime();

	/**
	* Pauses the Timeline, preventing the Timeline from updating
	*/
	void pause();

	/**
	* Unpauses the Timeline, allowing it to move again if paused before
	*/
	void unpause();

	/**
	* Returns whether or not the Timeline is paused
	* 
	* @returns Returns true if the Timeline is paused, false otherwise
	*/
	bool isPaused();

	/**
	* Sets reverse to true, reverses the time flow by returning negative time values
	*/
	void reverse();

	/**
	* Sets reverse to false, setting time values back to positive if reversed before
	*/
	void unreverse();

	/**
	* Returns whether or not the Timeline is reversed
	* 
	* @returns Returns true if the time flow of the Timeline is reversed, false otherwise
	*/
	bool isReversed();

	/**
	* Changes the tic of the current Timeline
	* 
	* @param tic: The tic value to be set to this Timeline
	*/
	void changeTic(int64_t tic);

	/**
	* Gets the time scale
	*
	* @return timeScale: The multiplier to set to affect the flow of time
	*/
	float getTimeScale();

	/**
	* Changes the multiplier applied to the flow of time in order to speed up or slow
	* down the current flow of time.
	* 
	* @param timeScale: The multiplier to set to affect the flow of time
	*/
	void changeTimeScale(float timeScale);
};
