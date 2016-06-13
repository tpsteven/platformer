#include "FrameTimer.hpp"

#include <cassert>
#include <iostream>
using namespace std;

#include <SDL.h>

#include "Log.hpp"
#include "Types.hpp"

FrameTimer::FrameTimer()
{
	// intentionally empty
}

FrameTimer::~FrameTimer()
{
	// intentionally empty
}

bool
FrameTimer::init(unsigned char windowSize)
{
	this->windowSize = windowSize;
	
	return true;
}

void
FrameTimer::reset()
{
	currFrame = 0;
	frameDeltas = queue<uint32_t>();
	frameCount = 0;
	ticking = false;
}

void
FrameTimer::start()
{
	// check that timer isn't already ticking
	assert(!ticking);
	ticking = true;
	
	lastFrame = SDL_GetTicks();
}

void
FrameTimer::stop()
{
	// check that timer is ticking
	assert(ticking);
	ticking = false;
}

void
FrameTimer::tick()
{
	// check that timer is ticking
	assert(ticking);
	++frameCount;
	
	// Update the frame delta and push onto the queue
	currFrame = SDL_GetTicks();
	frameDeltas.push(currFrame - lastFrame);
	frameDeltaTotal += currFrame - lastFrame;
	lastFrame = currFrame;
	
	// If queue contains more times than the window size, remove the front
	if (frameDeltas.size() > windowSize) {
		frameDeltaTotal -= frameDeltas.front();
		frameDeltas.pop();
	}
	
	if (frameDeltaTotal != 0) {
		// Only update fps if there is data to show
		fps = 1000 * (static_cast<float>(frameDeltas.size()) / frameDeltaTotal);
	}
	else if (frameDeltas.size() == windowSize) {
		// Since window size isn't enough to capture frame rate info, double
		// This avoids issues at high frame rates when the game runs so fast that
		//   frame times appear to be 0.
		windowSize <<= 1;
		msg << "Doubled window size to " << windowSize 
			 << " to capture high frame rates";
		Log::instance()->debug("FrameTimer::tick()", msg);
	}
}
