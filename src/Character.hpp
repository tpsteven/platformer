#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SDL.h>

#include "Types.hpp"

class Character {
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors/Destructor (defined in Character.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Create a Character whose lower left corner is at (x, y) in world
	 * coordinates and set its width and height to blockWidth
	 */
	Character(int x, int y, uint32_t blockSize);
	
	/**
	 * Clean up any memory allocations made by the Character
	 */
	~Character();

	////////////////////////////////////////////////////////////////////////////
	// Public mutator functions (defined in Character.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Move the Character to the specified destination while remaining within
	 * the specified bounds
	 */
	void setPosition(const FPair& dest, const SDL_Rect& bounds);
	
	/**
	 * Shift the Character by the specified amount while remaining within the
	 * specified bounds
	 */
	void shiftPosition(const FPair& delta, const SDL_Rect& bounds);

	////////////////////////////////////////////////////////////////////////////
	// Public accessor functions (defined here, not in Character.cpp)
	////////////////////////////////////////////////////////////////////////////

	/** 
	 * Return an unmodifiable reference to the true position
	 */
	const FPair& getPos() const {
		return pos;
	}

	/** 
	 * Return an unmodifiable reference to the dimensions and display position
	 */
	const SDL_Rect& getRect() const {
		return rect;
	}

private:
	FPair    pos;   // true position (world coordinates)
	SDL_Rect rect;  // dimensions and display position (world coordinates)
};

#endif

