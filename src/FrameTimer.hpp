#ifndef FRAME_TIMER_HPP
#define FRAME_TIMER_HPP

#include <iostream>
#include <queue>
using namespace std;

#include <SDL.h>

#include "Types.hpp"

class FrameTimer {
public:
	FrameTimer(unsigned char windowSize);
	~FrameTimer();

	void reset();
	void start();
	void stop();
	void tick();

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
	bool            ticking = false;
	float           fps = -1.0;
	queue<uint32_t> frameDeltas;
	uint32_t        currFrame       = 0;
	uint32_t        frameCount      = 0;
	uint32_t        frameDeltaTotal = 0;
	uint32_t        lastFrame       = 0;
	uint32_t        windowSize;
};

#endif

