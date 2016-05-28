#include "Platform.hpp"

#include <SDL.h>

Platform(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

Platform::Platform(const SDL_Rect& rect)
{
	this->rect.x = rect.x;
	this->rect.y = rect.y;
	this->rect.w = rect.w;
	this->rect.h = rect.h;
}

Platform::~Platform()
{
	// intentionally empty
}
