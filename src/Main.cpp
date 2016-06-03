#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include <SDL.h>

#include "Camera.hpp"
#include "FrameTimer.hpp"
#include "Log.hpp"
#include "Physics.hpp"
#include "Render.hpp"
#include "Scene.hpp"
#include "Types.hpp"

void loadArgs(int argc, char* argv[], RenderConfig* renderConfig);
RenderConfig* loadRenderConfig();
void pollInput(Input& input, bool& run);

int main(int argc, char* argv[])
{
	// Const values
	const uint32_t PS_SCALE = 40;  // screen-pixel to sprite-pixel scale

	// Load RenderConfig from file
	RenderConfig* renderConfig = loadRenderConfig();

	// Get command-line arguments
	loadArgs(argc, argv, renderConfig);

	// TODO: read list of levels from lvl/lvl.index, allow user to make selection

	bool run = true;
	FrameTimer frameTimer(renderConfig->frame_timer_window);
	Input input;
	Render renderer(PS_SCALE);
	Physics physics;
	Scene scene(PS_SCALE);

	// Initialize the renderer (including SDL and required libraries)
	if (!renderer.init()) {
		return -1;
	}

	// Create a window
	renderer.createWindow("platformer", renderConfig);

	// Create a camera (TODO: move to scene loading)
	Camera camera(renderer.getWidth(), renderer.getHeight());

	// TODO: load scene
	for (int i = 1; i < 16; ++i) {
		scene.addPlatform(i * 4, (i - 1) * 2, 3, 1);
	}
	
	scene.setBounds(0, 0, 64, 36);
	
/*
	scene.addPlatform(0, 0, 1, 1);
	scene.addPlatform(0, 17, 1, 1);
	scene.addPlatform(63, 0, 1, 1);
	scene.addPlatform(63, 17, 1, 1);
	scene.setBounds(0, 0, 64, 18);
*/

	frameTimer.start();

	// Game loop
	while (run) {
		// Input
		pollInput(input, run);

		// Physics 
		physics.step(scene, camera, input, frameTimer.getLastFrameTime());

        // Render
        renderer.render(scene, camera);

		// Update FPS
        frameTimer.tick();
		if (renderConfig->show_fps && frameTimer.getFrameCount() % 100 == 0) {
			renderer.updateFps(frameTimer.getFps());
		}
	}

	// Clean up
	if (renderConfig != nullptr) {
		delete renderConfig;
	}

    return 0;
}

void loadArgs(int argc, char* argv[], RenderConfig* renderConfig)
{
	stringstream error;

	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];

		if (arg.find('=') == string::npos) {
			// argument is a flag
			if (arg.compare("sw") == 0) {
				renderConfig->hardware_accelerated = false;
			}
			else {
				error << "Unknown arg (flag): " << arg;
				Log::warning("Main::loadArgs()", error);
			}
		}
		else {
			// argument is a key-value pair (separated by '=')
			string k = arg.substr(0, arg.find('='));
			string v = arg.substr(arg.find('=') + 1);
			
			// Check key-value settings here
			if (k.compare("fullscreen") == 0) {
				renderConfig->fullscreen = v.compare("true") == 0;
			}
			else if (k.compare("show_fps") == 0) {
				renderConfig->show_fps = v.compare("true") == 0;
			}
			else if (k.compare("hardware_accelerated") == 0) {
				renderConfig->hardware_accelerated = v.compare("true") == 0;
			}
			else {
				error << "Unknown arg (key-value pair): " << arg;
				Log::warning("Main::loadArgs()", error);
			}
		}
	}
}

RenderConfig* loadRenderConfig()
{
	RenderConfig* r = new RenderConfig();

	string line;
	stringstream error;
	fstream in(r->filename, ios_base::in);
	
	while(getline(in, line)) {
		if (line.find('=') == string::npos) {
			// argument is a flag
			error << "Unknown arg (flag): " << line;
			Log::warning("Main::loadRenderConfig()", error);
		}
		else {
			// argument is a key-value pair (separated by '=')
			string k = line.substr(0, line.find('='));
			string v = line.substr(line.find('=') + 1);
			
			// Check key-value settings here
			if (k.compare("background_camera") == 0) {
				r->background_camera = atoi(v.c_str());
			}
			else if (k.compare("background_level") == 0) {
				r->background_level = atoi(v.c_str());
			}
			else if (k.compare("frame_timer_window") == 0) {
				r->frame_timer_window = atoi(v.c_str());
			}
			else if (k.compare("fullscreen") == 0) {
				r->fullscreen = v.compare("true") == 0;
			}
			else if (k.compare("hardware_accelerated") == 0) {
				r->hardware_accelerated = v.compare("true") == 0;
			}
			else if (k.compare("show_fps") == 0) {
				r->show_fps = v.compare("true") == 0;
			}
			else if (k.compare("window_height") == 0) {
				r->window_height = atoi(v.c_str());
			}
			else if (k.compare("window_width") == 0) {
				r->window_width = atoi(v.c_str());
			}
			else {
				error << "Unknown arg (key-value pair): " << line;
				Log::warning("Main::loadRenderConfig()", error);
			}
		}
	}

	return r;
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

