#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <string>
using namespace std;

#include "Platform.hpp"
#include "Types.hpp"

class Scene {
public:
	Scene(uint32_t bwScale);
	~Scene();
	
	void load(string sceneName);
	void reset();
	void resetCurrent();

	// platform coordinates in block-coordinates	
	void addPlatform(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	void addPlatform(const SDL_Rect& rect);

	void setBounds(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	void setBounds(const SDL_Rect& rect);

	const list<Platform>& getPlatforms() const {
		return platforms;
	}

	const SDL_Rect& getBounds() const {
		return bounds;
	}

	const SDL_Rect& getWorldBounds() const {
		return worldBounds;
	}
	
private:
	list<Platform> platforms;
	SDL_Rect bounds;
	SDL_Rect worldBounds;
	string currentName;
	uint32_t bwScale;
	
	// doubly-linked list of Chunk nodes (deallocate when death laser passes)
};

#endif

