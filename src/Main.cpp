#include <iostream>
#include <string>
using namespace std;

#include <SDL.h>

#include "Camera.hpp"
#include "RenderSystem.hpp"
#include "Scene.hpp"
#include "Types.hpp"

int main(int argc, char* argv[])
{
	const char     SCREEN_TITLE[] = "platformer";
	const uint32_t SCREEN_WIDTH   = 1920;
	const uint32_t SCREEN_HEIGHT  = 1080;
	const uint32_t PS_SCALE       = 40;  // screen-pixel to sprite-pixel scale

	// TODO: get SCREEN_WIDTH, SCREEN_HEIGHT from args or resolution
	// TODO: read settings from cfg/ (indexed at cfg/cfg.index)

	Camera cam(SCREEN_WIDTH, SCREEN_HEIGHT);
	RenderSystem renderer(SCREEN_WIDTH, SCREEN_HEIGHT, PS_SCALE);
	SDL_Event e;

	bool run = true;    // Run until false
	bool sw = false;    // Default to hardware-accelerated renderer

	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];

		if (arg.find('=') == string::npos) {
			// argument is a flag
			if (arg.compare("sw") == 0) {
				sw = true;
			}
		}
		else {
			// argument is a key-value pair (separated by '=')
			string k = arg.substr(0, arg.find('='));
			string v = arg.substr(arg.find('=') + 1);
			
			// Check key-value settings here
		}
	}

	// Initialize the renderer (including SDL and required libraries)
	if (!renderer.init(SCREEN_TITLE, sw)) {
		return -1;
	}

	// TODO: read list of levels from lvl/lvl.index

	// Load scene
	Scene s;

	for (int i = 1; i < 16; ++i) {
		s.addPlatform(i * 4, (i - 1) * 2, 3, 1);
	}

	// Game loop
	while (run) {
		// Handle input events
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				// Exit button pressed
				run = false;
				break;
			case SDL_KEYDOWN:
				// Check key presses
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
        renderer.render(s, cam);
	}

    return 0;
}
