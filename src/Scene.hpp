#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
using namespace std;

class Scene {
public:
	Scene();
	~Scene();
	
	void load(string sceneName);
	void reset();
	void resetCurrent();
	
private:
	string currentName;
	// doubly-linked list of Chunk nodes (deallocate when death laser passes)
};

#endif

