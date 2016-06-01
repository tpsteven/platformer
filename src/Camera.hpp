#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SDL.h>

#include "Character.hpp"
#include "Types.hpp"

class Camera {
public:
	Camera(int x, int y, uint32_t width, uint32_t height);
	~Camera();

	void centerOnCharacter(const Character& character, const SDL_Rect& bounds);

	// negative numbers allowed for centering Camera around small Scenes
	void setPosition(const FPair& dest, const SDL_Rect& bounds);

	void shiftPosition(const FPair& dest, const SDL_Rect& bounds);

	const SDL_Rect& getRect() const {
		return rect;
	}
	
private:
	FPair    pos;   // true position (world coordinates)
	SDL_Rect rect;  // dimensions and display position (world coordinates)
};

#endif

