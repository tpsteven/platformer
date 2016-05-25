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
FrameTimer::start()
{
	// check that timer isn't already ticking
	assert(!ticking);
	ticking = true;
	
	// clear all values
	frameDeltas = queue<uint32_t>();
	currFrameTime = 0;
	frameCount = 0;
	lastFrameTime = SDL_GetTicks();
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
	
	currFrameTime = SDL_GetTicks();
	frameDeltas.push(currFrameTime - lastFrameTime);
	frameDeltaTotal += currFrameTime - lastFrameTime;
	lastFrameTime = currFrameTime;

	if (frameDeltas.size() > windowSize) {
		frameDeltaTotal -= frameDeltas.front();
		frameDeltas.pop();
	}
	
	if (frameDeltas.size() == windowSize) {
		fps = 1000 * windowSize / frameDeltaTotal;
	}
}

