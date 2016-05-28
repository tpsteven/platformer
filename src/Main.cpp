#include <iostream>
#include <string>
using namespace std;

#include <SDL.h>

#include "Camera.hpp"
#include "FrameTimer.hpp"
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
	FrameTimer frameTimer(100);
	RenderSystem renderer(SCREEN_WIDTH, SCREEN_HEIGHT, PS_SCALE);
	SDL_Event e;
	Scene s;

	bool fps = false;   // Display fps counter
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
			if (k.compare("fps") == 0) {
				fps = v.compare("true") == 0 ? true : false;
			}
		}
	}

	// Initialize the renderer (including SDL and required libraries)
	if (!renderer.init(SCREEN_TITLE, sw)) {
		return -1;
	}

	// TODO: read list of levels from lvl/lvl.index

	// TODO: load scene
	for (int i = 1; i < 16; ++i) {
		s.addPlatform(i * 4, (i - 1) * 2, 3, 1);
	}

	// Start the frame timer
	frameTimer.start();

	// Game loop
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	
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
				case SDLK_LEFT:
					left = true;
					break;
				case SDLK_RIGHT:
					right = true;
					break;
				case SDLK_UP:
					up = true;
					break;
				case SDLK_DOWN:
					down = true;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				// check key releases
				switch (e.key.keysym.sym) {
					case SDLK_LEFT:
						left = false;
						break;
					case SDLK_RIGHT:
						right = false;
						break;
					case SDLK_UP:
						up = false;
						break;
					case SDLK_DOWN:
						down = false;
						break;
					default:
						break;
				}
				break;
            default:
            	break;
			}
        }

		// Physics Stuff
		int camDifX = frameTimer.getLastFrameTime() / 2;
		int camDifY = frameTimer.getLastFrameTime() / 2;
		
		if (left && !right) {
			camDifX *= -1;
		}
		else if (!left && !right) {
			camDifX = 0;
		}

		if (down && !up) {
			camDifY *= -1;
		}
		else if (!down && !up) {
			camDifY = 0;
		}

		cam.shiftPosition(camDifX, camDifY);

        // Render
        renderer.render(s, cam);
	
		// Update frame time and display
		frameTimer.tick();
		if (fps && frameTimer.getFrameCount() % 100 == 0) {
			cout << frameTimer.getFps() << endl;
		}
	}

    return 0;
}
