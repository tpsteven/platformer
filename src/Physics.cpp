#include "Physics.hpp"

#include <cmath>
#include <iostream>
#include <list>
using namespace std;

#include "Camera.hpp"
#include "Character.hpp"
#include "Input.hpp"
#include "Scene.hpp"
#include "Types.hpp"

Physics::Physics()
	: framesSinceReleasedB(-1)
{
	// intentionally empty
}

Physics::~Physics()
{
	// intentionally empty
}


bool
Physics::step(Scene& scene, 
              Character& player,
              Camera& camera, 
              Input& input, 
              uint32_t lastFrameTime)
{
	FPair oldPosition(player.getPos().x, player.getPos().y);
	FPair delta = parseInput(scene, player, input, lastFrameTime);
    FPair correctionPair;
    
    correctionPairs.clear();
	
	// Move the player based on the input
	if (!player.shiftPosition(delta, scene.getCollisionBounds())) {
        // Part of player is outside scene bounds, dies violently
        return false;
    }

    // Check collisions and add necessary corrections to list
    for (const Platform& p : scene.getPlatforms()) {
		correctionPair = checkCollision(player, p.getRect());
		if (correctionPair.x != 0 || correctionPair.y != 0)
			correctionPairs.push_back(correctionPair);
    }

	if (correctionPairs.size() == 0) {
		player.touchingGround = false;
	}

	correctionPair = { 0, 0 };
	
	// Find greatest correction in each direction
	for (const FPair& f : correctionPairs) {
		if (abs(f.x) > abs(correctionPair.x)) {
			correctionPair.x = f.x;
		}
		
		if (abs(f.y) > abs(correctionPair.y)) {
			correctionPair.y = f.y;
		}
	}
	
	// Update player kinematic state	
	if (correctionPair.y > 0) {
		player.touchingGround = true;
		player.setVelocity(player.getVel().x, 0);
	}
	else if (correctionPair.y < 0) {
		player.setVelocity(player.getVel().x, 0);
	}
	
	if (correctionPair.x != 0) {
		player.setVelocity(0, player.getVel().y);
	}
	
	// Add the correction
	player.shiftPosition(correctionPair);

	// Center the camera on the player
	camera.centerOnCharacter(player, scene.getBounds());
    
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Private helper functions (defined in Physics.cpp)
////////////////////////////////////////////////////////////////////////////////
 
FPair
Physics::checkCollision(Character& p, const SDL_Rect& r)
{
	FPair delta;
    FPair v = p.getVel();
	SDL_Rect intersection;

	
	if (!(p.getPos().x                 < r.x + r.w &&
	      p.getPos().x + p.getRect().w > r.x       &&
		  p.getPos().y                 < r.y + r.h &&
	      p.getPos().y + p.getRect().h > r.y)) {
		return { 0, 0 };
	}
	
	if (SDL_IntersectRect(&p.getRect(), &r, &intersection) == SDL_FALSE) {
		return { 0 , 0 };
	}
	
	// Get horizontal overlap
	if (p.getPos().x > r.x) {
		delta.x = r.x + r.w - p.getPos().x;
	}
	else {
		delta.x = p.getPos().x + p.getRect().w - r.x;
	}
	
	// Get vertical overlap
	if (p.getPos().y > r.y) {
		delta.y = r.y + r.h - p.getPos().y;
	}
	else {
		delta.y = p.getPos().y + p.getRect().h - r.y;
	}
	
	// Set proper horizontal correction direction
	if (v.x > 0) {
		delta.x *= -1;
	}
	
	// Set proper vertical correction direction
	if (v.y > 0) {
		delta.y *= -1;
	}
	
	// Check intersections along horizontal edges
	if (intersection.w == p.getRect().w || intersection.w == r.w) {
		delta.x = 0;
		return delta;
	}
	
	// Check intersections along vertical edges
	if (intersection.h == p.getRect().h || intersection.h == r.h) {
		delta.y = 0;
		return delta;
	}
	
	// Intersection is at corner
	if (v.y < 0) {
		delta.x = 0;
		return delta;
	}
	
	if (abs(delta.y) > abs(delta.x)) {
		delta.y = 0;
		return delta;
	}
	else {
		delta.x = 0;
		return delta;
	}
}

FPair
Physics::parseInput(const Scene& scene,
                    Character& player, 
                    Input& input,
                    uint32_t lastFrameTime)
{
	float g = -3000.0f;
	float j = 1000.0f;
	ButtonEvent e;
	FPair velocity = { player.getVel().x, player.getVel().y };
	
	while (input.pollEvent(e)) {
		switch (e.button) {
			case Button::A:	
				if (e.state == ButtonState::Pressed && player.touchingGround) {
					velocity.y = j;
					// player.touchingGround = false;
				}
				
				break;
				
			default:
				break;
		}
	}
	
	if (player.touchingGround) {
		velocity.x = 320;
		
		if (input.getLeft() && !input.getRight()) {
			// Left pressed, so move left
			velocity.x *= -1;
		}
		else if ((input.getLeft() && input.getRight()) ||
				(!input.getLeft() && !input.getRight())) {
			// Both or neither pressed, so don't move horizontally
			velocity.x = 0;
		}
	}
	
	player.setVelocity(velocity.x, velocity.y + g * lastFrameTime / 1000.0f);
	
	// Move the player
	return { player.getVel().x * lastFrameTime / 1000.0f,
	         player.getVel().y * lastFrameTime / 1000.0f } ;
}
