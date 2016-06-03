#include "Render.hpp"

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.hpp"
#include "Log.hpp"
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
	// Destroy renderer
	if (renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
	}
	
	// Destroy window
	if (window != nullptr) {
		SDL_DestroyWindow(window);
	}
	
	// Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool
Render::createWindow(const char* title, const RenderConfig* renderConfig)
{
	if (window != nullptr) {
		Log::error("Render::createWindow()", "Window already exists");
	}

	// Set title
	this->title = title;

	// Create window
	uint32_t windowFlags = 0;

	if (renderConfig->fullscreen) {
		windowFlags |= SDL_WINDOW_FULLSCREEN;
		windowed = false;
		
		SDL_DisplayMode displayMode;
		if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
			Log::error("Render::createWindow()", SDL_GetError());
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
		Log::error("Render::createWindow()", SDL_GetError());
		return false;
	}

	// Create renderer for window
	uint32_t rendererFlags = SDL_RENDERER_SOFTWARE;

	if (renderConfig->hardware_accelerated) {
		rendererFlags = SDL_RENDERER_ACCELERATED;
	}

	renderer = SDL_CreateRenderer(window, -1, rendererFlags);
	
	// If unsuccessful: default to software renderer without vsync
	if (renderer == NULL) {
		Log::warning("Render::createWindow()",
		             "Falling back to software renderer without vsync");
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

		if (renderer == NULL) {
			Log::error("Render::createWindow()", SDL_GetError());
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
		Log::error("Render::init()", SDL_GetError());
		return false;
	}
	
    // Attempt to initialize IMG library
    // If unsuccessful, print error code and return
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		Log::error("Render::init()", IMG_GetError());
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
	stringstream error;

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL) {
		error << "(" << path << ") " << IMG_GetError();
		Log::error("Render::loadTexture()", error); 
	}
	else {
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL) {
			error << "(" << path << ") " << SDL_GetError();
			Log::error("Render::loadTexture()", error);
		}
		
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	
	return newTexture;
}

