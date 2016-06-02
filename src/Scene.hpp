#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <string>
using namespace std;

#include "Platform.hpp"
#include "Types.hpp"

class Scene {
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors/Destructor (defined in Scene.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Create a default scene with the specified blockSize
	 */
	Scene(uint32_t blockSize);
	
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
	void load(string sceneName);
	
	/**
	 * Reset the player position, enemy positions, etc.
	 */
	void reset();

	void setBounds(const SDL_Rect& rect);

	const list<Platform>& getPlatforms() const {
		return platforms;
	}

	const SDL_Rect& getBounds() const {
		return bounds;
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

	list<Platform> platforms;
	SDL_Rect bounds;
	SDL_Rect blockBounds;
	string currentName;
	uint32_t blockSize;
	
	// doubly-linked list of Chunk nodes (deallocate when death laser passes)
};

#endif

