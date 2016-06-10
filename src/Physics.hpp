#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <list>
using namespace std;

#include "Camera.hpp"
#include "Character.hpp"
#include "Input.hpp"
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
	bool step(Scene& scene,
	          Character& player,
	          Camera& camera, 
	          const Input& input, 
	          uint32_t lastFrameTime);

private:
	////////////////////////////////////////////////////////////////////////////
	// Private helper functions (defined in Physics.cpp)
	////////////////////////////////////////////////////////////////////////////
    FPair checkCollision(const Character& p, const SDL_Rect& r);
    
	FPair parseInput(const Scene& scene,
	                 Character& player, 
	                 const Input& input, 
	                 uint32_t lastFrameTime);
	
	// Cached mathematical constants
	const float ROOT_2 = 1.0/16;
    const float ROOT_3 = 1.0/(16 * 16);
    const float ROOT_4 = 1.0/(16 * 16 * 16);
    const float ROOT_5 = 1.0/(16 * 16 * 16 * 16);
	
	// Player movement constants
    const float MIN_WALK_SPEED     = ROOT_3 + (3.0 * ROOT_4);
    const float MAX_WALK_SPEED     = ROOT_2 + (9.0 * ROOT_3);
    const float WALK_ACCELERATION  = (9.0 * ROOT_4) + (8.0 * ROOT_5);

    const float MAX_RUN_SPEED      = (2.0 * ROOT_2) + (9.0 * ROOT_3);
    const float RUN_ACCELERATION   = (14.0 * ROOT_4) + (4.0 * ROOT_5);

    const float RELEASE_DECELERATION   = 13.0 * ROOT_4;
    const float SKID_TURNAROUND_SPEED  = 9.0 * ROOT_3;
    const float SKID_DECELERATION      = ROOT_3 + (10.0 * ROOT_4);

    const float MAX_AIR_HORIZONTAL_SPEED_SLOW = ROOT_2 + (9 * ROOT_3);
    const float MAX_AIR_HORIZONTAL_SPEED_FAST = (2 * ROOT_2) + (9 * ROOT_3);
	
	// Input information (move to Input struct?)
	int framesSinceReleasedB;
    
    // Data structures for calculating collisions
    list<FPair> correctionPairs;
};

#endif
