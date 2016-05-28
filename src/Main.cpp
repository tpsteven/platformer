#include <iostream>
#include <string>
using namespace std;

#include <SDL.h>

#include "Camera.hpp"
#include "RenderSystem.hpp"
#include "Types.hpp"

const char     SCREEN_TITLE[] = "platformer";
const uint32_t SCREEN_WIDTH   = 1920;
const uint32_t SCREEN_HEIGHT  = 1080;
const uint32_t PS_SCALE       = 2;  // screen-pixel to sprite-pixel scale

int main(int argc, char* argv[])
{
	Camera cam(SCREEN_WIDTH, SCREEN_HEIGHT);
	RenderSystem renderer(SCREEN_WIDTH, SCREEN_HEIGHT, PS_SCALE);
	SDL_Event e;

	bool run = true;    // Run until false
	bool sw = false;    // Default to hardware-accelerated renderer

	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];

		if (arg.find('=') == string::npos) {
			if (arg.compare("sw") == 0) {
				sw = true;
			}
		}
		else {
			string k = arg.substr(0, arg.find('='));
			string v = arg.substr(arg.find('=') + 1);
			
			// Check key-value settings here
		}
	}

	// Initialize the renderer (including SDL and required libraries)
	if (!renderer.init(SCREEN_TITLE, sw)) {
		return -1;
	}
	
	// Load level

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
        renderer.render(cam);
	}

    return 0;
}
