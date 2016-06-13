#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <sstream>
#include <string>
using namespace std;

#include "Platform.hpp"
#include "Types.hpp"

class Scene {
public:
	////////////////////////////////////////////////////////////////////////////
	// Types of Scenes
	////////////////////////////////////////////////////////////////////////////
	enum SceneType { ARENA_DEFAULT, ENDLESS };
	
	////////////////////////////////////////////////////////////////////////////
	// Constructors/Destructor (defined in Scene.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Create a default scene with the specified blockSize
	 */
	Scene();
	
	/**
	 * Clean up any memory allocations made by the Scene
	 */
	~Scene();
	
	////////////////////////////////////////////////////////////////////////////
	// Public mutator functions (defined in Scene.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Adds a new platform to the scene, increasing the horizontal bounds as
	 * necessary to the right
	 * 
	 * The platform must not have negative coordinates.
	 */
	void addPlatform(const SDL_Rect& rect);
	
	void addRandomPlatform(int width);
	
	/**
	 * Release the current resources and load a new scene.
	 */
	void load(SceneType type, uint32_t blockSize);
	
	/**
	 * Reset the player position, enemy positions, etc.
	 */
	void reset();

	void setBounds(const SDL_Rect& rect);

	const list<Platform>& getPlatforms() const {
		return platforms;
	}

	uint32_t getBlockSize() const {
		return blockSize;
	}
	
	const SDL_Rect& getBounds() const {
		return bounds;
	}
	
	const SDL_Rect& getCollisionBounds() const {
		return collisionBounds;
	}
	
private:
	/**
	 * Adds a new platform to the scene, increasing bounds as needed in either
	 * direction
	 * 
	 * The platform must not have negative coordinates.
	 */
	bool addPlatform(const SDL_Rect& rect, bool allowVerticalExpansion);

	bool expandBounds(const SDL_Rect& rect, bool allowVerticalExpansion);

	bool expandHorizontally;    // as the player approaches the right edge of the
	                            //   scene, should platforms be added?
	list<Platform> platforms;
	SceneType currentType;
	SDL_Rect blockBounds;
	SDL_Rect bounds;
	SDL_Rect collisionBounds;	// if player exits these boundaries, s/he dies
	uint32_t blockSize;
	
	stringstream msg;
};

#endif

