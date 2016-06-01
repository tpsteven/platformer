#include "Character.hpp"

Character::Character(int x, int y, uint32_t width, uint32_t height)
{
	pos.x = x;
	pos.y = y;

	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
}

Character::~Character()
{
	// intentionally empty
}


void 
Character::setPosition(float x, float y, const SDL_Rect& bounds)
{
	
}

void 
Character::shiftPosition(float dx, float dy, const SDL_Rect& bounds)
{
	setPosition(pos.x + dx, pos.y + dy, bounds);
}
