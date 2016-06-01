#ifndef RENDER_HPP
#define RENDER_HPP

#include <sstream>
#include <string>
using namespace std;

#include <SDL.h>

#include "Camera.hpp"
#include "Character.hpp"
#include "Scene.hpp"
#include "Types.hpp"

class Render{
public:
	Render(uint32_t bwScale);
	~Render();

	bool closeWindow();
	bool createWindow(const char* title, const RenderConfig* renderConfig);
	bool init();
	void render(const Scene& scene, 
	            const Character& player, 
	            const Camera& camera);
	void updateFps(float fps);

	int getHeight() const {
		return height;
	}

	int getWidth() const {
		return width;
	}

private:
	SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

	const uint32_t bwScale; // size of block, in world-pixels

	bool          windowed;
	int           height;
	int           width;
	SDL_Renderer* renderer = nullptr;
	SDL_Window*   window = nullptr;
	string        title;
	stringstream  titleStream;
};

#endif

