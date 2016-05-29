#include "PhysicsSystem.hpp"

PhysicsSystem::PhysicsSystem()
{
	// intentionally empty
}

PhysicsSystem::~PhysicsSystem()
{
	// intentionally empty
}


void
PhysicsSystem::step(Scene& scene, 
                    Camera& camera, 
                    const Input& input, 
                    uint32_t lastFrameTime)
{
	int camDifX = lastFrameTime / 2;
	int camDifY = lastFrameTime / 2;
		
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
