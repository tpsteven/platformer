SDL2_CFG = `sdl2-config --cflags --libs` -lSDL2_image
FILES = src/*.cpp 

game:
	g++ -std=c++11 -Wall $(FILES) -o platformer $(SDL2_CFG)

clean:
	rm platformer
