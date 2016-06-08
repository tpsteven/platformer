#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//////////////// Yuji /////////////////////
#ifdef RPI
	#include <iostream>
	#include <unistd.h>
	#include <errno.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include "GPIOClass.hpp"
	#include "GPController.hpp"
#endif
/////////////// End Yuji ///////////////////

#include <SDL.h>

#include "Camera.hpp"
#include "Character.hpp"
#include "FrameTimer.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Physics.hpp"
#include "Render.hpp"
#include "Scene.hpp"
#include "Types.hpp"

// Helper functions
void loadArgs(int argc, char* argv[], RenderConfig* renderConfig);
RenderConfig* loadRenderConfig();
void pollKeyboardInput(Input& input, bool& run);

// Global const values
const uint32_t BLOCK_SIZE = 20;  // size of block in world coordinates (pixels)

int main(int argc, char* argv[])
{
	// Load RenderConfig from file
	RenderConfig* renderConfig = loadRenderConfig();

	// Get command-line arguments and apply to renderConfig
	loadArgs(argc, argv, renderConfig);

	// Declare and define variables for game loop
	bool run = true;
	float lastLoggedTime = 0.0f;    // in seconds
	FrameTimer frameTimer(renderConfig->frame_timer_window);
	Input input;
	Render renderer(BLOCK_SIZE);
	Physics physics;
	Scene scene(BLOCK_SIZE);        // default constructor initializes
									//   a default scene

	// Initialize the renderer (including SDL and required libraries)
	if (!renderer.init()) {
		// If fails, terminate the program
		return -1;
	}

	// Create a window and set renderer width/height (later passed to Camera)
	renderer.createWindow("platformer", renderConfig);

	// Create a camera 
	Camera camera(0, 0, renderer.getWidth(), renderer.getHeight());

	// Create a player
	Character player(0, 0, BLOCK_SIZE);

	// Start frameTimer (so Physics can get frame times)
	frameTimer.start();

//////////////// Yuji /////////////////////
#ifdef RPI
	//Initialize GPIO pins
	controller.GPController;
#endif
/////////////// End Yuji ///////////////////
	
	// Game loop
	while (run) {
		// Get input, change run to false if necessary
		pollKeyboardInput(input, run);

//////////////// Yuji /////////////////////
	#ifdef RPI
			//get inputs, change states
		pollController(input, run);
	#endif
/////////////// End Yuji ///////////////////
		
		// Apply input to player, move player and check for collisions, move camera
		physics.step(scene, player, camera, input, frameTimer.getLastFrameTime());

        // Draw the environment and player to the window
        renderer.render(scene, player, camera);

		// Update the FPS display every 0.5 seconds, if enabled in renderConfig
        frameTimer.tick();
		if (renderConfig->show_fps && frameTimer.getTime() - lastLoggedTime > 0.5) {
			renderer.updateFps(frameTimer.getFps());
			lastLoggedTime = frameTimer.getTime();
		}
	}

	// Clean up pointers
	if (renderConfig != nullptr) {
		delete renderConfig;
	}

    return 0;
}

/**
 * Get command-line arguments and override current renderConfig settings.
 */
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

/**
 * Load RenderConfig from a file (RenderConfig defined in Types.hpp)
 */
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

/**
 * Get keyboard/mouse input using SDL and apply to the Input struct (defined
 * in Types.hpp)
 */
void pollKeyboardInput(Input& input, bool& run)
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
						input.pushEvent(Button::Down, ButtonState::Pressed);
						break;
						
					case SDLK_LEFT:
						input.pushEvent(Button::Left, ButtonState::Pressed);
						break;
						
					case SDLK_LSHIFT:
						input.pushEvent(Button::B, ButtonState::Pressed);
						break;
						
					case SDLK_RIGHT:
						input.pushEvent(Button::Right, ButtonState::Pressed);
						break;
			
					case SDLK_SPACE:
						input.pushEvent(Button::A, ButtonState::Pressed);
						break;
			
					case SDLK_UP:
						input.pushEvent(Button::Up, ButtonState::Pressed);
						break;
					
					default:
						break;
				}
				
				break;
				
			case SDL_KEYUP:     // Check key releases
				switch (e.key.keysym.sym) {
					case SDLK_DOWN:
						input.pushEvent(Button::Down, ButtonState::Released);
						break;
						
					case SDLK_LEFT:
						input.pushEvent(Button::Left, ButtonState::Released);
						break;
						
					case SDLK_LSHIFT:
						input.pushEvent(Button::B, ButtonState::Released);
						break;
						
					case SDLK_RIGHT:
						input.pushEvent(Button::Right, ButtonState::Released);
						break;
			
					case SDLK_SPACE:
						input.pushEvent(Button::A, ButtonState::Released);
						break;
			
					case SDLK_UP:
						input.pushEvent(Button::Up, ButtonState::Released);
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
//////////////// Yuji /////////////////////
#ifdef RPI
//pull button states and send to input
void pollController (Input& input, bool& run){
	while (!controller.inputList.empty()) {

		switch (controller.inputList.front().buttonUpDown) {

			case 1:   // Check key presses
				switch (controller.inputList.front().button) {
					case 'Yquit':
						run = false;
						break;

					case 'Xpin':
						input.down = true;	//temp values until input complete!!!!!
						break;

					case 'Ajump':
						input.left = true;	//temp values until input complete!!!!!
						break;

					case 'Bpin':
						input.right = true;	//temp values until input complete!!!!!
						break;

					case SDLK_UP: 			//replace with joystick values
						input.up = true;
						break;

					default:
						break;
				}

				break;

			case 0:     // Check key releases
				switch (controller.inputList.front().button) {
					case 'Xpin':
						input.down = false;	//temp values until input complete!!!!!
						break;

					case 'Ajump':
						input.left = false;	//temp values until input complete!!!!!
						break;

					case 'Bpin':
						input.right = false;	//temp values until input complete!!!!!
						break;

					case SDLK_UP: 			//replace with joystick values
						input.up = false;
						break;

					default:
						break;
				}

				break;

    		default:
           		break;
		}
		controller.inputList.pop_front();
    }
}
#endif
/////////////// End Yuji ///////////////////
