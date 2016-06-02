#include "Scene.hpp"

#include <SDL.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <list>
#include <string>
using namespace std;

#include "Types.hpp"

Scene::Scene(uint32_t blockSize)
	: currentName(""), blockSize(blockSize)
{
	// Initialize the random number generator
	srand(time(NULL));
	
	SDL_Rect rect { 0, 0, 8, 8 };
	setBounds(rect);
	
	rect.w = 3;
	rect.h = 1;
	
	// create a default scene
	for (int i = 1; i < 16; ++i) {
		rect.x = i * 4;
		rect.y = (i - 1) * 3;
		addPlatform(rect, true);
	}
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
Scene::load(string sceneName)
{
	currentName = sceneName;
	
	// TODO
}

void
Scene::reset()
{
	currentName = "";
	platforms.clear();

	// TODO: reset player position, enemy states, etc
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
}
