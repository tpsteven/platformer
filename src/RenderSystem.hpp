#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <sstream>
#include <string>
using namespace std;

#include <SDL.h>

#include "Camera.hpp"
#include "FrameTimer.hpp"
#include "Scene.hpp"
#include "Types.hpp"

class RenderSystem {
public:
	RenderSystem(uint32_t width, uint32_t height, uint32_t bwScale);
	~RenderSystem();

	bool init(const char* title, bool sw);
	void render(const Scene& scene, const Camera& cam);

private:
	SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

	const uint32_t width;
	const uint32_t height;
	const uint32_t bwScale; // size of block, in world-pixels

	FrameTimer*   frameTimer;
	SDL_Renderer* renderer = nullptr;
	SDL_Window*   window = nullptr;
	string        title;
};

#endif

