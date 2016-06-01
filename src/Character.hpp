#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <SDL.h>

#include "Types.hpp"

class Character {
public:
	Character(int x, int y, uint32_t width, uint32_t height);
	~Character();

	void setPosition(const FPair& dest, const SDL_Rect& bounds);
	void shiftPosition(const FPair& delta, const SDL_Rect& bounds);

	const FPair& getPos() const {
		return pos;
	}

	const SDL_Rect& getRect() const {
		return rect;
	}

private:
	FPair    pos;
	SDL_Rect rect;
};

#endif

