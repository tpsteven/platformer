#include "Camera.hpp"

#include <cmath>
#include <iostream>
using namespace std;

#include <SDL.h>

#include "Character.hpp"
#include "Types.hpp"

Camera::Camera()
{
	// intentionally empty
}

Camera::~Camera()
{
	// intentionally empty
}

void
Camera::centerOnCharacter(const Character& character, const SDL_Rect& bounds)
{
	// get the center of the character
	FPair corner(character.getPos().x + character.getRect().w / 2,
	             character.getPos().y + character.getRect().h / 2);
		
	corner.x -= rect.w / 2.0;
	corner.y -= rect.h / 2.0;
	
	setPosition(corner, bounds);
}

bool
Camera::init(uint32_t width, uint32_t height)
{
	pos.x = 0;
	pos.y = 0;
	
	rect.x = 0;
	rect.y = 0;
	rect.w = width;
	rect.h = height;
	
	return true;
}

void
Camera::setPosition(const FPair& dest, const SDL_Rect& bounds)
{
	// set x-position
	if (rect.w >= bounds.w) {
		rect.x = bounds.x - (rect.w - bounds.w) / 2;
		pos.x = rect.x;
	}
	else {
		if (dest.x <= bounds.x) {
			rect.x = bounds.x;
			pos.x = rect.x;
		}
		else if (dest.x + rect.w >= bounds.x + bounds.w) {
			rect.x = bounds.x + bounds.w - rect.w;
			pos.x = rect.x;
		}
		else {
			pos.x = dest.x;
			rect.x = round(pos.x);
		}
	}

	// set y-position
	if (rect.h >= bounds.h) {
		rect.y = bounds.y - (rect.h - bounds.h) / 2;
		pos.y = rect.y;
	}
	else {
		if (dest.y <= bounds.y) {
			rect.y = bounds.y;
			pos.y = rect.y;
		}
		else if (dest.y + rect.h >= bounds.y + bounds.h) {
			rect.y = bounds.y + bounds.h - rect.h;
			pos.y = rect.y;
		}
		else {
			pos.y = dest.y;
			rect.y = round(pos.y);
		}
	}
}

void 
Camera::shiftPosition(const FPair& delta, const SDL_Rect& bounds)
{
	FPair corner(pos.x + delta.x, pos.y + delta.y);
	setPosition(corner, bounds);
}
