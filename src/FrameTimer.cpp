#include "FrameTimer.hpp"

#include <cassert>
using namespace std;

#include <SDL.h>

#include "Types.hpp"

FrameTimer::FrameTimer(unsigned char windowSize)
	: windowSize(windowSize)
{
	// intentionally empty
}

FrameTimer::~FrameTimer()
{
	// intentionally empty
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
	
	currFrame = SDL_GetTicks();
	frameDeltas.push(currFrame - lastFrame);
	frameDeltaTotal += currFrame - lastFrame;
	lastFrame= currFrame;

	if (frameDeltas.size() > windowSize) {
		frameDeltaTotal -= frameDeltas.front();
		frameDeltas.pop();
	}
	
	fps = 1000 * frameDeltas.size() / frameDeltaTotal;
}

