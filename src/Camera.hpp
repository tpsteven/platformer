#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <SDL.h>

#include "Character.hpp"
#include "Types.hpp"

class Camera {
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors/Destructor (defined in Camera.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Create a Camera whose lower left corner is at (x, y) in world coordinates
	 * and set its width and height
	 */
	Camera(int x, int y, uint32_t width, uint32_t height);
	
	/**
	 * Clean up any memory allocations made by the Camera
	 */
	~Camera();
	
	////////////////////////////////////////////////////////////////////////////
	// Public mutator functions (defined in Camera.cpp)
	////////////////////////////////////////////////////////////////////////////
    
	/**
	 * Given a character with a true position, width, and height, center on the
	 * character's center while remaining within the specified bounds
	 */
	void centerOnCharacter(const Character& character, const SDL_Rect& bounds);
	
	/**
	 * Move the Camera to the specified destination while remaining within the
	 * specified bounds
	 */
	void setPosition(const FPair& dest, const SDL_Rect& bounds);
	
	/**
	 * Shift the Camera by the specified amount while remaining within the
	 * specified bounds
	 */
	void shiftPosition(const FPair& delta, const SDL_Rect& bounds);

	////////////////////////////////////////////////////////////////////////////
	// Public accessor functions (defined here, not in Camera.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/** 
	 * Return an unmodifiable reference to the true position
	 */
	const FPair& getPos() const {
		return pos;
	}
	
	/** 
	 * Return an unmodifiable reference to the dimensions and display position
	 */
	const SDL_Rect& getRect() const {
		return rect;
	}
	
private:
	FPair    pos;   // true position (world coordinates)
	SDL_Rect rect;  // dimensions and display position (world coordinates)
};

#endif

