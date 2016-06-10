#include "Character.hpp"

#include <cmath>
#include <iostream>
using namespace std;

Character::Character()
{
	// Intentionally empty
}

Character::~Character()
{
	// Intentionally empty
}

bool
Character::init(int x, int y, uint32_t blockSize)
{
	pos.x = x;
	pos.y = y;

	rect.x = x;
	rect.y = y;
	rect.w = blockSize;
	rect.h = blockSize;
	
	return true;
}

void 
Character::setPosition(const FPair& dest)
{
	pos.x = dest.x;
	rect.x = round(pos.x);
	
	pos.y = dest.y;
	rect.y = round(pos.y);
}

bool 
Character::setPosition(const FPair& dest, const SDL_Rect& collisionBounds)
{	
	if (dest.x < collisionBounds.x ||
	    dest.y < collisionBounds.y ||
		dest.x + rect.w >= collisionBounds.x + collisionBounds.w ||
		dest.y + rect.h >= collisionBounds.y + collisionBounds.h) 
	{
		return false;
	}
	
	// Set x-position
	if (dest.x <= collisionBounds.x) {
		// Bounded on left
		rect.x = collisionBounds.x;
		pos.x = rect.x;
	}
	else if (dest.x + rect.w >= collisionBounds.x + collisionBounds.w) {
		// Bounded on right
		rect.x = collisionBounds.x + collisionBounds.w - rect.w;
		pos.x = rect.x;
	}
	else {
		// Not bounded
		pos.x = dest.x;
		rect.x = round(pos.x);
	}

	// set y-position
	if (dest.y <= collisionBounds.y) {
		// Bounded on bottom
		rect.y = collisionBounds.y;
		pos.y = rect.y;
	}
	else if (dest.y + rect.h >= collisionBounds.y + collisionBounds.h) {
		// Bounded on top
		rect.y = collisionBounds.y + collisionBounds.h - rect.h;
		pos.y = rect.y;
	}
	else {
		// Not bounded
		pos.y = dest.y;
		rect.y = round(pos.y);
	}
	
	// If they intersect, return true
	return true;
}

void
Character::shiftPosition(const FPair& delta)
{
	FPair corner(pos.x + delta.x, pos.y + delta.y);
	setPosition(corner);
}

bool 
Character::shiftPosition(const FPair& delta, const SDL_Rect& collisionBounds)
{
	FPair corner(pos.x + delta.x, pos.y + delta.y);
	return setPosition(corner, collisionBounds);
}
