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
Camera::setPosition(uint32_t x, uint32_t y)
{
	rect.x = x;
	rect.y = y;
}
