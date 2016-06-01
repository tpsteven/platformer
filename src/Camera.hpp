#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SDL.h>

#include "Types.hpp"

class Camera {
public:
	Camera(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	~Camera();

	void centerOnPosition(float x, float y, const SDL_Rect& bounds);

	// negative numbers allowed for centering Camera around small Scenes
	void setPosition(float x, float y, const SDL_Rect& bounds);

	void shiftPosition(float x, float y, const SDL_Rect& bounds);

	const SDL_Rect& getRect() const {
		return rect;
	}
	
private:
	FPair    pos;   // true position (world coordinates)
	SDL_Rect rect;  // dimensions and display position (world coordinates)
};

#endif

