SDL2_LINUX = `sdl2-config --cflags --libs` -lSDL2_image

WIN_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
WIN_INC = -Isrc\sdl\SDL2\i686-w64-mingw32\include\SDL2 -Isrc\sdl\SDL2_image\i686-w64-mingw32\include\SDL2
WIN_LIB = -Lsrc\sdl\SDL2\i686-w64-mingw32\lib -Lsrc\sdl\SDL2_image\i686-w64-mingw32\lib
SDL2_WINDOWS = $(WIN_INC) $(WIN_LIB) $(WIN_FLAGS) 

FILES = src/*.cpp 

linux:
	g++ -std=c++11 -Wall $(FILES) -o platformer $(SDL2_LINUX)

rpi:
	g++ -DRPI -std=c++11 -Wall $(FILES) -o platformer $(SDL2_LINUX)
	
windows:
	g++ $(FILES) $(WIN_INC) $(WIN_LIB) -std=c++11 -Wall $(WIN_FLAGS) -o win\platformer

clean:
	rm platformer
