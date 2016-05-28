#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <string>
using namespace std;

#include "Platform.hpp"

class Scene {
public:
	Scene();
	~Scene();
	
	void load(string sceneName);
	void reset();
	void resetCurrent();

	// platform coordinates in block-coordinates	
	void addPlatform(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	void addPlatform(const SDL_Rect& rect);

	const list<Platform>& getPlatforms() const {
		return platforms;
	}
	
private:
	list<Platform> platforms;
	string currentName;
	
	// doubly-linked list of Chunk nodes (deallocate when death laser passes)
};

#endif

