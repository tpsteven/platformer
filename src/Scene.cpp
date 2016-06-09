#include "Scene.hpp"

#include <SDL.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <string>
using namespace std;

#include "Types.hpp"

Scene::Scene()
{
	// Initialize the random number generator
	srand(time(NULL));
}

Scene::~Scene()
{
	// intentionally empty
}

void
Scene::addPlatform(const SDL_Rect& rect)
{
	addPlatform(rect, false);
}

bool
Scene::addPlatform(const SDL_Rect& rect, bool allowVerticalExpansion)
{
	// TODO: insert in sorted order (ordered by x)

	assert(rect.x >= 0);
	assert(rect.y >= 0);

	if (!expandBounds(rect, allowVerticalExpansion)) {
		return false;
	}
	
	platforms.emplace_back(rect, blockSize);
	return true;
}

void
Scene::addRandomPlatform(int width)
{
	SDL_Rect platform { blockBounds.w, rand() % blockBounds.h, width, 1};
	assert(addPlatform(platform, false));
}

bool
Scene::expandBounds(const SDL_Rect& rect, bool allowVerticalExpansion)
{
	int farX = rect.x + rect.w;
	int farY = rect.y + rect.h;
	
	if (farY > blockBounds.h) {
		if (!allowVerticalExpansion) {
			return false;
		}
		
		blockBounds.h = farY;
		bounds.h = blockBounds.h * blockSize;
	}
	
	if (farX > blockBounds.w) {
		blockBounds.w = farX;
		bounds.w = blockBounds.w * blockSize;
	}

	return true;
}

void
Scene::load(SceneType type, uint32_t blockSize)
{	
	this->blockSize = blockSize;
	currentType = type;
	
	switch (type) {
		case ARENA_DEFAULT:
			cout << "Loading ARENA_DEFAULT: ";
			expandHorizontally = false;
		
			// Set scene boundaries
			setBounds({ 0, 0, 40, 20 });
			
			// Create level boundaries
			addPlatform({0, 0, 22, 1});	  // Bottom left
			addPlatform({26, 0, 14, 1});  // Bottom right
			addPlatform({0, 9, 1, 10});   // Left top
			addPlatform({0, 3, 1, 3});    // Left bottom
			addPlatform({39, 13, 1, 6});  // Right top
			addPlatform({39, 1, 1, 10});  // Right bottom
			addPlatform({0, 19, 6, 1});   // Top left
			addPlatform({8, 19, 18, 1});  // Top middle
			addPlatform({30, 19, 10, 1}); // Top right
			
			// Add some platforms
			for (int i = 1; i < 6; ++i) {
				addPlatform({ i * 5, i * 3, 3, 1 }, true);
			}
			
			// Add some more platforms
			addPlatform({20, 4, 14, 1});
			addPlatform({35, 9, 3, 1});
			addPlatform({30, 12, 3, 1});
			
			cout << "COMPLETE" << endl;
			break;
			
		case ENDLESS:
			expandHorizontally = true;
			
			break;
			
		default:
			break;
	}
}

void
Scene::reset()
{
	platforms.clear();
	load(currentType, blockSize);
}

void
Scene::setBounds(const SDL_Rect& rect)
{
	blockBounds.x = rect.x;
	blockBounds.y = rect.y;
	blockBounds.w = rect.w;
	blockBounds.h = rect.h;
	
	bounds.x = blockBounds.x * blockSize;
	bounds.y = blockBounds.y * blockSize;
	bounds.w = blockBounds.w * blockSize;
	bounds.h = blockBounds.h * blockSize;
	
	collisionBounds.x = -3 * blockSize;
	collisionBounds.y = -3 * blockSize;
	collisionBounds.w = (blockBounds.w + 6) * blockSize;
	collisionBounds.h = (blockBounds.h + 12) * blockSize;
}
