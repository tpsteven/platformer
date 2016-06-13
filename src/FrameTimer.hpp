#ifndef FRAME_TIMER_HPP
#define FRAME_TIMER_HPP

#include <iostream>
#include <queue>
#include <sstream>
using namespace std;

#include <SDL.h>

#include "Types.hpp"

class FrameTimer {
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors/Destructor (defined in FrameTimer.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 *  Create a FrameTimer with the specified window size
	 */
	FrameTimer();
	
	/**
	 * Clean up any memory allocations made by the FrameTimer
	 */  
	~FrameTimer();

	////////////////////////////////////////////////////////////////////////////
	// Public mutator functions (defined in Camera.cpp)
	////////////////////////////////////////////////////////////////////////////

	/**
	 * Initialize the FrameTimer with windowSize
	 */
	bool init(unsigned char windowSize);
	
	/**
	 * Stop the timer and clear all recorded frame times
	 */
	void reset();
	
	/**
	 * Start the timer and prepare to record frame times
	 */
	void start();
	
	/**
	 * Stop the timer and disable frame time recording
	 */
	void stop();
	
	/**
	 * Record the next frame time (currFrame - lastFrame)
	 */
	void tick();

	////////////////////////////////////////////////////////////////////////////
	// Public accessor functions (defined here, not in Camera.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	float getFps() const {
		return fps;
	}

	float getLastFrameTime() const {
		return (frameCount == 0) ? 0 : frameDeltas.back();
	}

	uint32_t getFrameCount() const {
		return frameCount;
	}
	
	bool getTicking() const {
		return ticking;
	}

	float getTime() const {
		return static_cast<float>(currFrame) / 1000;
	}

private:
	bool            ticking = false;        // is the timer tracking time?
	float           fps = -1.0;             // current frames per second
	queue<uint32_t> frameDeltas;            // list of the last windowSize frame
											//   frame times (ms)
	uint32_t        currFrame       = 0;    // current frame time (measured once
											//   per frame
	uint32_t        frameCount      = 0;    // total frame count
	uint32_t        frameDeltaTotal = 0;    // total sum of frameDeltas
	uint32_t        lastFrame       = 0;    // last measured frame time
	uint32_t        windowSize;             // number of frame times to use when
											//   calculating fps
									
	stringstream    msg;
};

#endif

