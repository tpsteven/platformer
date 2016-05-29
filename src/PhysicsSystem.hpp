#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Camera.hpp"
#include "Scene.hpp"
#include "Types.hpp"

class PhysicsSystem {
public:
	PhysicsSystem();
	~PhysicsSystem();

	void step(Scene& scene, 
	          Camera& camera, 
	          const Input& input, 
	          uint32_t lastFrameTime);
	
};

#endif

