#include <iostream>
#include <string>
using namespace std;

#include <SDL.h>

#include "Camera.hpp"
#include "FrameTimer.hpp"
#include "Physics.hpp"
#include "Render.hpp"
#include "Scene.hpp"
#include "Types.hpp"

void pollInput(Input& input, bool& run);

int main(int argc, char* argv[])
{
	const char     SCREEN_TITLE[] = "platformer";
	const uint32_t SCREEN_WIDTH   = 1920;
	const uint32_t SCREEN_HEIGHT  = 1080;
	const uint32_t PS_SCALE       = 40;  // screen-pixel to sprite-pixel scale

	// TODO: get SCREEN_WIDTH, SCREEN_HEIGHT from args or resolution
	// TODO: read settings from cfg/ (indexed at cfg/cfg.index)

	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);
	FrameTimer frameTimer(100);
	Input input;
	Render renderer(SCREEN_WIDTH, SCREEN_HEIGHT, PS_SCALE);
	Physics physics;
	Scene scene(PS_SCALE);

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
	/* for (int i = 1; i < 16; ++i) {
		s.addPlatform(i * 4, (i - 1) * 2, 3, 1);
	}*/
	//s.setBounds(0, 0, 64, 36);
	
	scene.addPlatform(0, 0, 1, 1);
	scene.addPlatform(0, 17, 1, 1);
	scene.addPlatform(63, 0, 1, 1);
	scene.addPlatform(63, 17, 1, 1);
	scene.setBounds(0, 0, 64, 18);

	// Start the frame timer
	frameTimer.start();

	// Game loop
	while (run) {
		// Input
		pollInput(input, run);

		// Physics 
		physics.step(scene, camera, input, frameTimer.getLastFrameTime());

        // Render
        renderer.render(scene, camera);
	
		// Update frame time and display
		frameTimer.tick();
		if (fps && frameTimer.getFrameCount() % 100 == 0) {
			cout << frameTimer.getFps() << endl;
		}
	}

    return 0;
}

void pollInput(Input& input, bool& run)
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
			case SDL_QUIT:      // Exit button pressed
				run = false;
				break;
				
			case SDL_KEYDOWN:   // Check key presses
				switch (e.key.keysym.sym) {
					case SDLK_q:
						run = false;
						break;
						
					case SDLK_DOWN:
						input.down = true;
						break;
						
					case SDLK_LEFT:
						input.left = true;
						break;
						
					case SDLK_RIGHT:
						input.right = true;
						break;
			
					case SDLK_UP:
						input.up = true;
						break;
						
					default:
						break;
				}
				
				break;
				
			case SDL_KEYUP:     // Check key releases
				switch (e.key.keysym.sym) {
					case SDLK_DOWN:
						input.down = false;
						break;
						
					case SDLK_LEFT:
						input.left = false;
						break;
						
					case SDLK_RIGHT:
						input.right = false;
						break;
						
					case SDLK_UP:
						input.up = false;
						break;
						
					default:
						break;
				}   
			
				break;
			
    		default:
           		break;
		}
    }
}

