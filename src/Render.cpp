#include "Render.hpp"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.hpp"
#include "Platform.hpp"
#include "Scene.hpp"
#include "Types.hpp"

Render::Render(uint32_t bwScale)
	: bwScale(bwScale)
{
	// intentionally empty
	// initialization occurs in Render::init()
}

Render::~Render()
{
	// Destroy window
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}
	
	// Destroy renderer
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}
	
	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool
Render::createWindow(const char* title, const RenderConfig* renderConfig)
{
	if (window != nullptr) {
		cout << "[ERROR] Render::createWindow() - window already exists" << endl;
	}

	// Set title
	this->title = title;

	// Create window
	uint32_t windowFlags = SDL_WINDOW_INPUT_GRABBED;

	if (renderConfig->fullscreen) {
		windowFlags |= SDL_WINDOW_FULLSCREEN;
		windowed = false;
		
		SDL_DisplayMode displayMode;
		if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
	     	cout << "SDL_GetDesktopDisplayMode failed: " 
				 << SDL_GetError() 
				 << endl;
			return 1;
		}

		width = displayMode.w;
		height = displayMode.h;
	}
	else {
		windowed = true;
		width = renderConfig->window_width;
		height = renderConfig->window_height;
	}

	window = SDL_CreateWindow(title,
	                          SDL_WINDOWPOS_CENTERED,
	                          SDL_WINDOWPOS_CENTERED,
	                          width,
	                          height,
	                          windowFlags);

	// If unsuccessful: print error code and return
	if (window == NULL) {
		cout << "Window could not be created: " <<  SDL_GetError() << endl;
		return false;
	}

	// Create renderer for window
	uint32_t rendererFlags = SDL_RENDERER_SOFTWARE;

	if (renderConfig->hardware_accelerated) {
		rendererFlags |= SDL_RENDERER_ACCELERATED;
	}

	renderer = SDL_CreateRenderer(window, -1, rendererFlags);
	
	// If unsuccessful: default to software renderer without vsync
	if (renderer == NULL) {
		cout << "Falling back to software renderer without vsync." << endl;
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

		if (renderer == NULL) {
			cout << "Renderer could not be created: " << SDL_GetError() << endl;
			return false;
		}
	}

	return true;
}

bool
Render::closeWindow()
{
	bool result = true;

	// Destroy window
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}
	else {
		result = false;
	}
	
	// Destroy renderer
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}
	else {
		result = false;
	}

	return result;
}

bool
Render::init()
{
	// Attempt to initialize SDL
	// If unsuccessful, print error code and return
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize: %s\n", SDL_GetError());
		return false;
	}
	
    // Attempt to initialize IMG library
    // If unsuccessful, print error code and return
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", 
		       IMG_GetError());
		return false;
	}
	
	return true;
}

void
Render::render(const Scene& scene, const Camera& camera)
{
	SDL_Rect intersectRect;
	SDL_Rect worldRect;

	auto blockToWorld = [&] (SDL_Rect& rect) {
		rect.x *= bwScale;
		rect.y *= bwScale;
		rect.w *= bwScale;
		rect.h *= bwScale;
	};

	auto camShift = [&] (SDL_Rect& rect) {
		rect.x -= camera.getRect().x;
		rect.y -= camera.getRect().y;
	};

	auto worldToSDL = [&] (SDL_Rect& rect) {
		rect.y = height - (rect.y + rect.h);
	};

	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// Draw level background
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	worldRect = scene.getBounds();
	blockToWorld(worldRect);
	SDL_IntersectRect(&worldRect, &camera.getRect(), &intersectRect);
	camShift(intersectRect);
	worldToSDL(intersectRect);
	SDL_RenderFillRect(renderer, &intersectRect);

	// Draw all platforms
	SDL_SetRenderDrawColor(renderer, 0x7d, 0xc1, 0xf0, 0xFF);
	for (Platform p : scene.getPlatforms()) {
		worldRect = p.getRect();
		blockToWorld(worldRect);

		if (SDL_IntersectRect(&worldRect, &camera.getRect(), &intersectRect) ==
		    SDL_TRUE) {
			camShift(intersectRect);
			worldToSDL(intersectRect);
			SDL_RenderFillRect(renderer, &intersectRect);
		}
	}

	SDL_RenderPresent(renderer);
}

void
Render::updateFps(float fps)
{
	if (windowed) {
		titleStream.str("");
		titleStream.clear();
			
		titleStream << title << " (" << fps << " fps)";
		SDL_SetWindowTitle(window, titleStream.str().c_str());
	}
	else {
		cout << fps << " fps" << endl;
	}
}

SDL_Texture* 
Render::loadTexture(const char* path, SDL_Renderer* renderer)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n",
		       path,
		       IMG_GetError());
	}
	else {
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", 
			       path,
			       SDL_GetError());
		}
		
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	
	return newTexture;
}

