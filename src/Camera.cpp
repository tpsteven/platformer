#include "Camera.hpp"

#include <cmath>
#include <iostream>
using namespace std;

#include <SDL.h>

Camera::Camera(uint32_t width, uint32_t height)
{
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;

	pos.x = 0.0f;
	pos.x = 0.0f;
}

Camera::~Camera()
{
	// intentionally empty
}

void
Camera::setPosition(float x, float y, const SDL_Rect& bounds)
{
	// set x-position
	if (rect.w >= bounds.w) {
		rect.x = bounds.x - (rect.w - bounds.w) / 2;
		pos.x = rect.x;
	}
	else {
		if (x <= bounds.x) {
			rect.x = bounds.x;
			pos.x = rect.x;
		}
		else if (x + rect.w >= bounds.x + bounds.w) {
			rect.x = bounds.x + bounds.w - rect.w;
			pos.x = rect.x;
		}
		else {
			pos.x = x;
			rect.x = round(pos.x);
		}
	}

	// set y-position
	if (rect.h >= bounds.h) {
		rect.y = bounds.y - (rect.h - bounds.h) / 2;
		pos.y = rect.y;
	}
	else {
		if (y <= bounds.y) {
			rect.y = bounds.y;
			pos.y = rect.y;
		}
		else if (y + rect.h >= bounds.y + bounds.h) {
			rect.y = bounds.y + bounds.h - rect.h;
			pos.y = rect.y;
		}
		else {
			pos.y = y;
			rect.y = round(pos.y);
		}
	}
}

void 
Camera::shiftPosition(float x, float y, const SDL_Rect& bounds)
{
	setPosition(pos.x + x, pos.y + y, bounds);
}
