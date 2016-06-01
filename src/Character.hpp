#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SDL.h>

#include "Types.hpp"

class Character {
public:
	Character(int x, int y, uint32_t width, uint32_t height);
	~Character();

	void setPosition(float x, float y, const SDL_Rect& bounds);
	void shiftPosition(float dx, float dy, const SDL_Rect& bounds);

private:
	FPair    pos;
	SDL_Rect rect;
};

#endif

