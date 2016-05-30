#include "Camera.hpp"

#include <iostream>
using namespace std;

#include <SDL.h>

Camera::Camera(uint32_t width, uint32_t height)
{
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;
}

Camera::~Camera()
{
	// intentionally empty
}

void
Camera::setPosition(int x, int y, const SDL_Rect& bounds)
{
	// set x-position
	if (rect.w >= bounds.w) {
		rect.x = bounds.x - (rect.w - bounds.w) / 2;
	}
	else {
		if (x <= bounds.x) {
			rect.x = bounds.x;
		}
		else if (x + rect.w >= bounds.x + bounds.w) {
			rect.x = bounds.x + bounds.w - rect.w;
		}
		else {
			rect.x = x;
		}
	}

	// set y-position
	if (rect.h >= bounds.h) {
		rect.y = bounds.y - (rect.h - bounds.h) / 2;
	}
	else {
		if (y <= bounds.y) {
			rect.y = bounds.y;
		}
		else if (y + rect.h >= bounds.y + bounds.h) {
			rect.y = bounds.y + bounds.h - rect.h;
		}
		else {
			rect.y = y;
		}
	}
}

void 
Camera::shiftPosition(int x, int y, const SDL_Rect& bounds)
{
	// rect.x = (rect.x + x < 0) ? 0 : rect.x + x;
	// rect.y = (rect.y + y < 0) ? 0 : rect.y + y;

	setPosition(rect.x + x, rect.y + y, bounds);
}
