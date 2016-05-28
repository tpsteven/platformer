#include "Camera.hpp"

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
Camera::setPosition(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void 
Camera::shiftPosition(int x, int y)
{
	rect.x = (rect.x + x < 0) ? 0 : rect.x + x;
	rect.y = (rect.y + y < 0) ? 0 : rect.y + y;
}
