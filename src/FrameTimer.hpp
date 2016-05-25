#ifndef FRAME_TIMER_HPP
#define FRAME_TIMER_HPP

#include <queue>
using namespace std;

#include "Types.hpp"

class FrameTimer {
public:
	FrameTimer(unsigned char windowSize);
	~FrameTimer();

	void start();
	void stop();
	void tick();

	float getFps() const {
		return fps;
	};

	uint32_t getFrameCount() const {
		return frameCount;
	}

private:
	bool            ticking = false;
	float           fps = -1.0;
	queue<uint32_t> frameDeltas;
	uint32_t        currFrameTime   = 0;
	uint32_t        frameCount      = 0;
	uint32_t        frameDeltaTotal = 0;
	uint32_t        lastFrameTime   = 0;
	uint32_t        windowSize;
};

#endif

