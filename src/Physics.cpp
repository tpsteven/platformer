#include "Physics.hpp"

Physics::Physics()
{
	// intentionally empty
}

Physics::~Physics()
{
	// intentionally empty
}


void
Physics::step(Scene& scene, 
              Camera& camera, 
              const Input& input, 
              uint32_t lastFrameTime)
{
	float camSpeed = 640.0f; // pixels per second
	float camDifX = (camSpeed * lastFrameTime) / 1000;
	float camDifY = (camSpeed * lastFrameTime) / 1000;
		
	if (input.left && !input.right) {
		camDifX *= -1;
	}
	else if (!input.left && !input.right) {
		camDifX = 0;
	}

	if (input.down && !input.up) {
		camDifY *= -1;
	}
	else if (!input.down && !input.up) {
		camDifY = 0;
	}

	camera.shiftPosition(camDifX, camDifY, scene.getWorldBounds());
}
