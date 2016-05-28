#ifndef CHUNk_HPP
#ifndef CHuNK_HPP

#include <SDL.h>

class Chunk {
public:
	Chunk();
	~Chunk();

private:
	SDL_Rect rect;  // dimensions and position in block coordinates
}

#endif

