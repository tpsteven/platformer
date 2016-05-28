#include "Scene.hpp"

#include <SDL.h>

#include <list>
#include <string>
using namespace std;

Scene::Scene()
	: currentName("")
{
	// intentionally empty
}

Scene::~Scene()
{
	// intentionally empty
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
Scene::resetCurrent()
{
	// TODO: reset player position, enemy states, etc
}

void
Scene::addPlatform(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
	// TODO: insert in sorted order (ordered by x)

	platforms.emplace_back(x, y, w, h);
}

void
Scene::addPlatform(const SDL_Rect& rect)
{
	// TODO: insert in sorted order (ordered by x)

	platforms.emplace_back(rect);
}
