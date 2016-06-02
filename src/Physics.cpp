#include "Physics.hpp"

#include <iostream>
using namespace std;

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
	float v = 320.0f;                          // velocity in pixels per second
	float vReal = v * lastFrameTime / 1000.0f; // v adjusted for frame rate
		
	// Get input and calculate player's next movement
	FPair delta(vReal, vReal); // velocity starts up and to the right
	
	if (input.left && !input.right) {
		// Left pressed, so move left
		delta.x *= -1;
	}
	else if ((input.left && input.right) ||
	         (!input.left && !input.right)) {
		// Both or neither pressed, so don't move horizontally
		delta.x = 0;
	}

	if (input.down && !input.up) {
		// Down pressed, so move down
		delta.y *= -1;
	}
	else if ((input.down && input.up) ||
	         (!input.down && !input.up)) {
		// Both or neither pressed, so don't move vertically
		delta.y = 0;
	}

	// Move the player (using the scene boundaries as boundaries)
	player.shiftPosition(delta, scene.getBounds());
	
	// Add more platforms as player gets close to the edge
	if (scene.getBounds().w - player.getRect().x < camera.getRect().w) {
		scene.addRandomPlatform(3);
		stepAddedLastFrame = true;
	}
	else if (stepAddedLastFrame) {
		cout << "Frame time: " << lastFrameTime << endl;
		stepAddedLastFrame = false;
	}
	
	// Center the camera on the player
	camera.centerOnCharacter(player, scene.getBounds());
}
