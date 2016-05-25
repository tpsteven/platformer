#include <SDL.h>

#include "RenderSystem.hpp"

const char SCREEN_TITLE[] = "platformer";
const int  SCREEN_WIDTH   = 1920;
const int  SCREEN_HEIGHT  = 1080;

int main(int argc, char* argv[])
{
	bool run = true;
	RenderSystem renderer;
	SDL_Event e;

	if (!renderer.init(SCREEN_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT)) {
		return -1;
	}
	
	// Game loop
	while (run) {
		// Handle input events
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				run = false;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
				case SDLK_q:
					run = false;
					break;
				default:
					break;
				}
            default:
            	break;
			}
        }

        // Render
        renderer.render();
	}

    return 0;
}
