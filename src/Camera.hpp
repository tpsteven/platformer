#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SDL.h>

#include "Types.hpp"

class Camera {
public:
	Camera(uint32_t width, uint32_t height);
	~Camera();

	// negative numbers allowed for centering Camera around small bounds
	void setPosition(int x, int y, const SDL_Rect& bounds);

	void shiftPosition(int x, int y, const SDL_Rect& bounds);

	const SDL_Rect& getRect() const {
		return rect;
	}
	
private:
	SDL_Rect rect;  // dimensions and position in pixel coordinates
};

#endif

