#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Camera.hpp"
#include "Character.hpp"
#include "Scene.hpp"
#include "Types.hpp"

class Physics {
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors/Destructor (defined in Physics.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Create a Physics system, used for making physics updates to a scene.
	 */
	Physics();
	
	/**
	 * Clean up any memory allocations made by the Physics system
	 */
	~Physics();

	////////////////////////////////////////////////////////////////////////////
	// Public functions (defined in Physics.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Moves the player character according to the input and last frame time, 
	 * checking for collisions with other objects in the Scene, and then moves
	 * the camera to follow the player.
	 */
	void step(Scene& scene,
	          Character& player,
	          Camera& camera, 
	          const Input& input, 
	          uint32_t lastFrameTime);

private:
	bool stepAddedLastFrame = false;
};

#endif
