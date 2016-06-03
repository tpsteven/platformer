#include "Physics.hpp"

#include <cmath>
#include <iostream>
using namespace std;

#include "Camera.hpp"
#include "Character.hpp"
#include "Input.hpp"
#include "Scene.hpp"

Physics::Physics()
	: framesSinceReleasedB(-1)
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
	FPair oldPosition(player.getPos().x, player.getPos().y);
	FPair newPosition;
	SDL_Rect intersection;
	
	// Move the player based on the input
	parseInput(scene, player, input, lastFrameTime);
	
	// Check collisions
	for (Platform p : scene.getPlatforms()) {
		if (SDL_IntersectRect(&p.getRect(), &player.getRect(), &intersection) ==
		    SDL_TRUE) {
			// TODO: look at horizontal and vertical velocities, determine 
			//   whether player would hit side or top/bottom first
			
			// TODO: fix case where player holds down and moves across bump
			// in two connected platforms (or just avoid two connected platforms
			// in the first place)
			if (intersection.w < intersection.h) {
				if (oldPosition.x < player.getPos().x) {
					newPosition.x = p.getRect().x - player.getRect().w;
				}
				else {
					newPosition.x = p.getRect().x + p.getRect().w;
				}
				
				newPosition.y = player.getPos().y;
			}
			else if (intersection.w > intersection.h) {
				if (oldPosition.y < player.getPos().y) {
					newPosition.y = p.getRect().y - player.getRect().h;
				}
				else {
					newPosition.y = p.getRect().y + p.getRect().h;
				}
				
				newPosition.x = player.getPos().x;
			}
			else {
				if (player.getRect().x == intersection.x) {
					newPosition.x = player.getPos().x + intersection.w;
				}
				else {
					newPosition.x = player.getPos().x - intersection.w;
				}
				
				if (player.getRect().y == intersection.y) {
					newPosition.y = player.getPos().y + intersection.h;
				}
				else {
					newPosition.y = player.getPos().y - intersection.h;
				}
			}
			
			player.setPosition(newPosition, scene.getBounds());
		}
	}
	
	// Add more platforms as player gets close to the edge
	if (scene.getBounds().w - player.getRect().x < camera.getRect().w) {
		scene.addRandomPlatform(3);
	}
	
	// Center the camera on the player
	camera.centerOnCharacter(player, scene.getBounds());
}

////////////////////////////////////////////////////////////////////////////////
// Private helper functions (defined in Physics.cpp)
////////////////////////////////////////////////////////////////////////////////
 
void
Physics::parseInput(const Scene& scene,
                    Character& player, 
                    const Input& input,
                    uint32_t lastFrameTime)
{
	float v = 320.0f;                          // velocity in pixels per second
	float vReal = v * lastFrameTime / 1000.0f; // v adjusted for frame rate
	FPair delta(vReal, vReal);                 // Velocity starts up and right
	
	if (input.getLeft() && !input.getRight()) {
		// Left pressed, so move left
		delta.x *= -1;
	}
	else if ((input.getLeft() && input.getRight()) ||
			(!input.getLeft() && !input.getRight())) {
		// Both or neither pressed, so don't move horizontally
		delta.x = 0;
	}
	
	if (input.getDown() && !input.getUp()) {
		// Down pressed, so move down
		delta.y *= -1;
	}
	else if ((input.getDown() && input.getUp()) ||
			(!input.getDown() && !input.getUp())) {
		// Both or neither pressed, so don't move vertically
		delta.y = 0;
	}
	
	// Move the player (using the scene boundaries as boundaries)
	player.shiftPosition(delta, scene.getBounds());
}