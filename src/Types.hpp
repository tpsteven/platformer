#ifndef TYPES_HPP
#define TYPES_HPP

typedef unsigned int uint32_t;
typedef unsigned char uchar_t;

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

#endif

