#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SDL.h>

#include "Types.hpp"

class Platform {
public:
	Platform(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	Platform(const SDL_Rect& rect);
	~Platform();

	const SDL_Rect& getRect() const {
		return rect;
	}

private:
	SDL_Rect rect;  // dimensions and position in block-coordinates
};

#endif

