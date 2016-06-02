#include "Character.hpp"

#include <cmath>
using namespace std;

Character::Character(int x, int y, uint32_t blockSize)
{
	pos.x = x;
	pos.y = y;

	rect.x = x;
	rect.y = y;
	rect.w = blockSize;
	rect.h = blockSize;
}

Character::~Character()
{
	// Intentionally empty
}

void 
Character::setPosition(const FPair& dest, const SDL_Rect& bounds)
{
	// Set x-position
	if (rect.w >= bounds.w) {
		// Center horizontally on the bounds' center
		rect.x = bounds.x - (rect.w - bounds.w) / 2;
		pos.x = rect.x;
	}
	else {
		if (dest.x <= bounds.x) {
			// Bounded on left
			rect.x = bounds.x;
			pos.x = rect.x;
		}
		else if (dest.x + rect.w >= bounds.x + bounds.w) {
			// Bounded on right
			rect.x = bounds.x + bounds.w - rect.w;
			pos.x = rect.x;
		}
		else {
			// Not bounded
			pos.x = dest.x;
			rect.x = round(pos.x);
		}
	}

	// set y-position
	if (rect.h >= bounds.h) {
		// Center vertically on the bounds' center
		rect.y = bounds.y - (rect.h - bounds.h) / 2;
		pos.y = rect.y;
	}
	else {
		if (dest.y <= bounds.y) {
			// Bounded on bottom
			rect.y = bounds.y;
			pos.y = rect.y;
		}
		else if (dest.y + rect.h >= bounds.y + bounds.h) {
			// Bounded on top
			rect.y = bounds.y + bounds.h - rect.h;
			pos.y = rect.y;
		}
		else {
			// Not bounded
			pos.y = dest.y;
			rect.y = round(pos.y);
		}
	}
}

void 
Character::shiftPosition(const FPair& delta, const SDL_Rect& bounds)
{
	FPair corner(pos.x + delta.x, pos.y + delta.y);
	setPosition(corner, bounds);
}
