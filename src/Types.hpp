#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
using namespace std;

typedef unsigned int uint32_t;
typedef unsigned char uchar_t;

/**
 * A simple struct that stores an (x, y) position
 */
struct FPair {
	float x;
	float y;

	FPair() : x(0.0f), y(0.0f) {}
	FPair(float x, float y) : x(x), y(y) {}
};

/**
 * A struct that contains all the input flags.
 * 
 * Values are updated in pollInput in the main loop, and the physics system uses
 * them to move the player character.
 */
struct Input {
	bool down  = false;
	bool left  = false;
	bool right = false;
	bool up    = false;

	bool a     = false;
	bool b     = false;
	bool x     = false;
	bool y     = false;
};

/**
 * A struct that contains render settings. Initialized in main().
 */
struct RenderConfig {
	const char* filename = "cfg/render.cfg";

	bool fullscreen = false;                // Default fullscreen mode
	bool hardware_accelerated = true;       // Default to hardware-accelerated 
	                                        //   renderer
	bool show_fps = false;                  // Display fps counter
	uint32_t background_camera = 0x751fff;  // Default camera background color
	uint32_t background_level = 0x000000;   // Default level background color
	uint32_t frame_timer_window = 100;      // Default frame timer window
	uint32_t window_height = 720;           // Default window height
	uint32_t window_width = 1280;           // Default window width
};

#endif

