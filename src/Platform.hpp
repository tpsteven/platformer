#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SDL.h>

#include "Types.hpp"

class Platform {
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors/Destructor (defined in Platform.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Create a Platform at the given SDL_Rect in block coordinates (which are
	 * then converted to world coordinates)
	 */
	Platform(const SDL_Rect& blockRect, uint32_t blockSize);
	
	/**
	 * Clean up any memory allocations made by the Platform
	 */
	~Platform();

	////////////////////////////////////////////////////////////////////////////
	// Public accessor functions (defined here, not in Platform.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	const SDL_Rect& getRect() const {
		return rect;
	}

private:
	SDL_Rect rect; // dimensions and position in world coordinates
};

#endif
