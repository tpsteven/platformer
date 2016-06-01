#include "Physics.hpp"

#include "Camera.hpp"
#include "Character.hpp"
#include "Scene.hpp"

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
              Character& player,
              Camera& camera, 
              const Input& input, 
              uint32_t lastFrameTime)
{
	float v = 640.0f; // pixels per second
	float vReal = v * lastFrameTime / 1000.0f;
	FPair delta(vReal, vReal);
		
	if (input.left && !input.right) {
		delta.x *= -1;
	}
	else if (!input.left && !input.right) {
		delta.x = 0;
	}

	if (input.down && !input.up) {
		delta.y *= -1;
	}
	else if (!input.down && !input.up) {
		delta.y = 0;
	}

	player.shiftPosition(delta, scene.getWorldBounds());
	camera.centerOnCharacter(player, scene.getWorldBounds());
}
