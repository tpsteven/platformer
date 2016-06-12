#include <cassert>
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
bool calculateBlockSize(uint32_t& blockSize,
                        uint32_t numBlocks, 
                        uint32_t windowHeight);
void loadRenderArgs(int argc, char* argv[], RenderConfig& renderConfig);
void loadRenderConfig(RenderConfig& renderConfig);
void pollKeyboardInput(Input& input, bool& run);

#ifdef RPI
	void pollController(GPController& controller, Input& input, bool& run);
#endif

int main(int argc, char* argv[])
{
	bool reset = false;
	bool run = true;                // game loop stops when false
	float lastLoggedTime = 0.0f;	// in seconds
	uint32_t blockSize = 0;         // size of block in pixels on screen
	
	Camera camera;
	Character player;
	FrameTimer frameTimer;
	Input input;
	Render renderer;
	RenderConfig renderConfig;
	Physics physics;
	Scene scene;
	
	// Load RenderConfig from file and apply command-line arguments
	loadRenderConfig(renderConfig);
	loadRenderArgs(argc, argv, renderConfig);

#ifdef RPI
	renderConfig.fullscreen=true;
	renderConfig.hardware_accelerated=true;
#endif

	// Initialize the renderer (including SDL and required libraries)
	assert(renderer.init());
	
	// Create a window and set renderer width/height (later passed to Camera)
	assert(renderer.createWindow("platformer", renderConfig));
	cout << "Window: (" << renderer.getWidth() << "," << renderer.getHeight() << ")" << endl;
	
	// Calculate the block size, assuming the scene is 20 blocks high
	assert(calculateBlockSize(blockSize, 20, renderer.getHeight()));
	
	// Initialize the camera 
	assert(camera.init(renderer.getWidth(), renderer.getHeight()));

	// Initialize the player
	assert(player.init(1, 1, blockSize));

	// Initialize the scene
	scene.load(Scene::ARENA_DEFAULT, blockSize);
	cout << "Scene size: (" << scene.getBounds().w << ", " << scene.getBounds().h << ")" << endl;
	
	// Initialize and start frameTimer (so Physics can get frame times)
	assert(frameTimer.init(renderConfig.frame_timer_window));
	frameTimer.start();

//////////////// Yuji /////////////////////
#ifdef RPI
	//Initialize GPIO pins
	GPController controller;
#endif
/////////////// End Yuji ///////////////////
	
	// Game loop
	while (run) {
		if (reset) {
			scene.reset();
			camera.centerOnCharacter(player, scene.getBounds());
			player.setPosition({ (float) blockSize, (float) blockSize });
			player.setVelocity({ 0, 0 });
			player.touchingGround = true;
			frameTimer.stop();
			renderer.render(scene, player, camera);
			SDL_Delay(500);
			pollKeyboardInput(input, reset);
			input.reset();
			frameTimer.start();
			SDL_Delay(17);
			frameTimer.tick();
			reset = false;
		}
		
//////////////// Yuji /////////////////////
	#ifdef RPI
			//get inputs, change states
		controller.pollController();
		pollController(controller, input, run);
	#endif
/////////////// End Yuji ///////////////////
		
		// Get input, change run to false if necessary
		pollKeyboardInput(input, run);

		// Apply input to player, move player and check for collisions, move camera
		if (!physics.step(scene, player, camera, input, frameTimer.getLastFrameTime())) {
			reset = true;
		}

        // Draw the environment and player to the window
        renderer.render(scene, player, camera);
		
		// Update the FPS display every 0.5 seconds, if enabled in renderConfig
        frameTimer.tick();
		if (renderConfig.show_fps && frameTimer.getTime() - lastLoggedTime > 0.5) {
			renderer.updateFps(frameTimer.getFps());
			lastLoggedTime = frameTimer.getTime();
		}
	}

    return 0;
}

/**
 * Calculate the necessary block height in pixels
 */
bool calculateBlockSize(uint32_t& blockSize, 
                        uint32_t numBlocks, 
                        uint32_t windowHeight)
{
	if (numBlocks <= 0 || windowHeight <= 0) {
		return false;
	}
	
	// 16 is the smallest possible block size
	blockSize = 16;
	
	// Increase the block size by 16 until 20 blocks don't fit in the window
	if (numBlocks * blockSize < windowHeight) {
		while ((blockSize + 16) * numBlocks <= windowHeight) {
			blockSize += 16;
		}
	}
	
	cout << "Block Size: " << blockSize << endl;
	
	return true;
}

/**
 * Get command-line arguments and override current renderConfig settings.
 */
void loadRenderArgs(int argc, char* argv[], RenderConfig& renderConfig)
{
	stringstream error;

	for (int i = 1; i < argc; ++i) {
		string arg = argv[i];

		if (arg.find('=') == string::npos) {
			// argument is a flag
			if (arg.compare("sw") == 0) {
				renderConfig.hardware_accelerated = false;
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
				renderConfig.fullscreen = v.compare("true") == 0;
			}
			else if (k.compare("show_fps") == 0) {
				renderConfig.show_fps = v.compare("true") == 0;
			}
			else if (k.compare("hardware_accelerated") == 0) {
				renderConfig.hardware_accelerated = v.compare("true") == 0;
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
void loadRenderConfig(RenderConfig& renderConfig)
{
	string line;
	stringstream error;
	fstream in(renderConfig.filename, ios_base::in);
	
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
				renderConfig.background_camera = atoi(v.c_str());
			}
			else if (k.compare("background_level") == 0) {
				renderConfig.background_level = atoi(v.c_str());
			}
			else if (k.compare("frame_timer_window") == 0) {
				renderConfig.frame_timer_window = atoi(v.c_str());
			}
			else if (k.compare("fullscreen") == 0) {
				renderConfig.fullscreen = v.compare("true") == 0;
			}
			else if (k.compare("hardware_accelerated") == 0) {
				renderConfig.hardware_accelerated = v.compare("true") == 0;
			}
			else if (k.compare("show_fps") == 0) {
				renderConfig.show_fps = v.compare("true") == 0;
			}
			else if (k.compare("window_height") == 0) {
				renderConfig.window_height = atoi(v.c_str());
			}
			else if (k.compare("window_width") == 0) {
				renderConfig.window_width = atoi(v.c_str());
			}
			else {
				error << "Unknown arg (key-value pair): " << line;
				Log::warning("Main::loadRenderConfig()", error);
			}
		}
	}
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
						if (e.key.repeat == 0)
							input.pushEvent(Button::Down, ButtonState::Pressed);
						break;
						
					case SDLK_LEFT:
						if (e.key.repeat == 0)
							input.pushEvent(Button::Left, ButtonState::Pressed);
						break;
						
					case SDLK_LSHIFT:
						if (e.key.repeat == 0)
							input.pushEvent(Button::B, ButtonState::Pressed);
						break;
						
					case SDLK_RIGHT:
						if (e.key.repeat == 0)
							input.pushEvent(Button::Right, ButtonState::Pressed);
						break;
			
					case SDLK_SPACE:
						if (e.key.repeat == 0)
							input.pushEvent(Button::A, ButtonState::Pressed);
						break;
			
					case SDLK_UP:
						if (e.key.repeat == 0)
							input.pushEvent(Button::Up, ButtonState::Pressed);
						break;
					
					case SDLK_LALT:
						if (e.key.repeat == 0)
							input.pushEvent(Button::Alt, ButtonState::Pressed);
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
						
					case SDLK_LALT:
						input.pushEvent(Button::Alt, ButtonState::Released);
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
void pollController (GPController& controller, Input& input, bool& run){

	for(int i=0;i<6;i++){
		if(controller.inputList[i]->get_buttonEvent()!=0){
							//cout << "buttonEvent = 0" << endl;

			switch(controller.inputList[i]->get_buttonUpDown()){
				case 1:   // Check key presses

					switch (controller.inputList[i]->get_button()){
						case 'Y':
							run = false;
							//cout << "Y = down" << endl;
							break;

						case 'L':
							input.pushEvent(Button::Left, ButtonState::Pressed);
							//cout << "L = down" << endl;
							break;

						case 'A':
							input.pushEvent(Button::A, ButtonState::Pressed);
							//cout << "A = down" << endl;
							break;

						case 'R':
							input.pushEvent(Button::Right, ButtonState::Pressed);
							//cout << "R = down" << endl;
							break;
						case 'B':
							input.pushEvent(Button::B, ButtonState::Pressed);
							//cout << "B = down" << endl;
							break;
						case 'X':
							input.pushEvent(Button::X, ButtonState::Pressed);
							//cout << "R = down" << endl;
							break;

						default:
							break;
					}

					break;

				case 0:     // Check key releases

					switch (controller.inputList[i]->get_button()){
						case 'L':
							input.pushEvent(Button::Left, ButtonState::Released);
							break;

						case 'A':
							input.pushEvent(Button::A, ButtonState::Released);
							break;

						case 'R':
							input.pushEvent(Button::Right, ButtonState::Released);
							break;
						case 'B':
							input.pushEvent(Button::B, ButtonState::Released);
							break;
						case 'X':
							input.pushEvent(Button::X, ButtonState::Released);
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
}
#endif
/////////////// End Yuji ///////////////////
