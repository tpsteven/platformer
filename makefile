SDL2_CFG = `sdl2-config --cflags --libs` -lSDL2_image

platformer:
	g++ -std=c++11 -Wall src/*.cpp -o platformer $(SDL2_CFG)

clean:
	rm platformer
