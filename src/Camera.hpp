#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SDL.h>

#include "Types.hpp"

class Camera {
public:
	Camera(uint32_t width, uint32_t height);
	~Camera();

	void setPosition(uint32_t x, uint32_t y);

	const SDL_Rect& getRect() const {
		return rect;
	}
	
private:
	SDL_Rect rect;  // dimensions and position in pixel coordinates
};

#endif

