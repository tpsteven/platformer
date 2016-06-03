#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
using namespace std;

typedef unsigned int uint32_t;
typedef unsigned char uchar_t;

struct FPair {
	float x = 0.0f;
	float y = 0.0f;
};

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

struct RenderConfig {
	const char* filename = "cfg/render.cfg";

	bool fullscreen = false;
	bool hardware_accelerated = true;       // Default to hardware-accelerated 
	                                        //   renderer
	bool show_fps = false;                  // Display fps counter
	uint32_t background_camera = 0x751fff;
	uint32_t background_level = 0x000000;
	uint32_t frame_timer_window = 100;
	uint32_t window_height = 720;
	uint32_t window_width = 1280;	
};

#endif

