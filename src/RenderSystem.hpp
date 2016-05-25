#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <sstream>
#include <string>
using namespace std;

#include <SDL.h>

#include "FrameTimer.hpp"
#include "Types.hpp"

class RenderSystem {
public:
	RenderSystem();
	~RenderSystem();

	bool init(const char* title, const int& width, const int& height);
	void render();

private:
	SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

	FrameTimer*   frameTimer;
	SDL_Renderer* renderer = nullptr;
	SDL_Window*   window = nullptr;
	string        title;
	uint32_t      width;
	uint32_t      height;
};

#endif

