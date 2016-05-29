#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "Camera.hpp"
#include "Scene.hpp"
#include "Types.hpp"

class Physics {
public:
	Physics();
	~Physics();

	void step(Scene& scene, 
	          Camera& camera, 
	          const Input& input, 
	          uint32_t lastFrameTime);
	
};

#endif

