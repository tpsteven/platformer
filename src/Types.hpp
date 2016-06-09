#ifndef TYPES_HPP
#define TYPES_HPP

#include <initializer_list>
#include <string>
using namespace std;

// Forward declarations
class Camera;
class Character;
class FrameTimer;
class Input;
class Physics;
class Render;

typedef unsigned int uint32_t;
typedef unsigned char uchar_t;

enum class Button {
	A, B, X, Y, Down, Left, Up, Right
};

enum class ButtonState {
	Pressed, Released
};

struct ButtonEvent {
	Button      button;
	ButtonState state;
	
	ButtonEvent(const Button& b, const ButtonState& s) : button(b), state(s) {}
	ButtonEvent(const ButtonEvent& e) : button(e.button), state(e.state) {}
};

/**
 * A simple struct that stores an (x, y) position
 */
struct FPair {
	float x;
	float y;

	FPair() : x(0.0f), y(0.0f) {}
	FPair(float x, float y) : x(x), y(y) {}
	
	FPair(const initializer_list<float>& il) {
		auto it = il.begin();
		x = *it++;
		y = *it;
	}
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

