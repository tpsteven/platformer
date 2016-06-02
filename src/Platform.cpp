#include "Platform.hpp"

#include <SDL.h>

#include "Types.hpp"

Platform::Platform(const SDL_Rect& blockRect, uint32_t blockSize)
{
	rect.x = blockRect.x * blockSize;
	rect.y = blockRect.y * blockSize;
	rect.w = blockRect.w * blockSize;
	rect.h = blockRect.h * blockSize;
}

Platform::~Platform()
{
	// intentionally empty
}
