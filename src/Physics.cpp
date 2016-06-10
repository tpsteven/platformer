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
        correctionPairs.push_back(checkCollision(player, p.getRect()));
    }
	
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
		player.touchingGround = false;
		player.setVelocity(player.getVel().x, 0);
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
	
	if (SDL_IntersectRect(&p.getRect(), &r, &intersection) == SDL_FALSE) {
		return { 0, 0 };
	}
	
	// Check intersections along horizontal edges
	if (intersection.w == p.getRect().w || intersection.w == r.w) {
		delta.x = 0;
		delta.y = (p.getRect().y > r.y) ? intersection.h : -1 * intersection.h;
		p.touchingGround = delta.y >= 0;
		return delta;
	}
	
	// Check intersections along vertical edges
	if (intersection.h == p.getRect().h || intersection.h == r.h) {
		delta.y = 0;
		delta.x = (p.getRect().x > r.x) ? intersection.w : -1 * intersection.w;
		return delta;
	}
	
	// Intersection is at corner
	if (abs(v.x) == abs(v.y)) {
		if (intersection.w < intersection.h) {
			delta.y = 0;
			delta.x = (p.getRect().x > r.x) ? intersection.w : -1 * intersection.w;
			return delta;
		}
		else {
			delta.x = 0;
			delta.y = (p.getRect().y > r.y) ? intersection.h : -1 * intersection.h;
			p.touchingGround = delta.y >= 0;
			return delta;
		}
	}
	else if (abs(v.y) > abs(v.x)) {
		delta.x = 0;
		delta.y = (p.getRect().y > r.y) ? intersection.h : -1 * intersection.h;
		p.touchingGround = delta.y >= 0;
		return delta;
	}
	else {
		delta.y = 0;
		delta.x = (p.getRect().x > r.x) ? intersection.w : -1 * intersection.w;
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
	FPair velocity = { 320, player.getVel().y };
	
	while (input.pollEvent(e)) {
		switch (e.button) {
			case Button::A:	
				if (e.state == ButtonState::Pressed && player.touchingGround) {
					velocity.y = j;
					player.touchingGround = false;
				}
				
				break;
				
			default:
				break;
		}
	}
	
	if (player.touchingGround) {
		player.setVelocity(player.getVel().x, 0);
	}
	
	if (input.getLeft() && !input.getRight()) {
		// Left pressed, so move left
		velocity.x *= -1;
	}
	else if ((input.getLeft() && input.getRight()) ||
			(!input.getLeft() && !input.getRight())) {
		// Both or neither pressed, so don't move horizontally
		velocity.x = 0;
	}
	
	player.setVelocity(velocity.x, velocity.y + g * lastFrameTime / 1000.0f);
	
	// Move the player
	return { player.getVel().x * lastFrameTime / 1000.0f,
	         player.getVel().y * lastFrameTime / 1000.0f } ;
}
