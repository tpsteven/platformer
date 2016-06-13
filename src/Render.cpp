#include "Render.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

#include <SDL.h>
#include <SDL_image.h>

#include "Camera.hpp"
#include "Character.hpp"
#include "Log.hpp"
#include "Platform.hpp"
#include "Scene.hpp"
#include "Types.hpp"

Render::Render()
	: renderer(nullptr), window(nullptr)
{
	// Intentionally empty, because initialization occurs in Render::init()
	// It's there because it could fail, and throwing an exception in a 
	//   constructor isn't great style.
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
Render::createWindow(const char* title, const RenderConfig& renderConfig)
{
	if (window != nullptr) {
		Log::instance()->error("Render::createWindow()", "Window already exists");
	}

	// Set title
	this->title = title;

	// Create window
	uint32_t windowFlags = 0;

	if (renderConfig.fullscreen) {
		windowFlags |= SDL_WINDOW_FULLSCREEN;
		fullscreen = true;
		
		SDL_DisplayMode displayMode;
		if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
			Log::instance()->error("Render::createWindow()", SDL_GetError());
			return 1;
		}

		width = displayMode.w;
		height = displayMode.h;
	}
	else {
		fullscreen = false;
		width = renderConfig.window_width;
		height = renderConfig.window_height;
	}

	window = SDL_CreateWindow(title,
	                          SDL_WINDOWPOS_CENTERED,
	                          SDL_WINDOWPOS_CENTERED,
	                          width,
	                          height,
	                          windowFlags);

	// If unsuccessful: print error code and return
	if (window == NULL) {
		Log::instance()->error("Render::createWindow()", SDL_GetError());
		return false;
	}

	// Create renderer for window
	uint32_t rendererFlags = SDL_RENDERER_SOFTWARE;

	if (renderConfig.hardware_accelerated) {
		rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(window, -1, rendererFlags);
	
	// If unsuccessful: default to software renderer without vsync
	if (renderer == NULL) {
		Log::instance()->warning("Render::createWindow()",
		             "Falling back to software renderer without vsync");
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

		if (renderer == NULL) {
			Log::instance()->error("Render::createWindow()", SDL_GetError());
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
		Log::instance()->error("Render::init()", SDL_GetError());
		return false;
	}
	
    // Attempt to initialize IMG library
    // If unsuccessful, print error code and return
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		Log::instance()->error("Render::init()", IMG_GetError());
		return false;
	}
	
	return true;
}

void
Render::render(const Scene& scene, const Character& player, const Camera& camera)
{
	SDL_Rect intersectRect; // for checking the intersection between an object
	                        //   and the camera
	SDL_Rect worldRect;     // cached object rect (to avoid calling getRect())
	
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	
	// Check where scene intersects the camera/window
	worldRect = scene.getBounds();
	SDL_IntersectRect(&worldRect, &camera.getRect(), &intersectRect);
	worldToSDL(intersectRect, camera.getRect());
	
	// Draw level background
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &intersectRect);
	
	// Draw all platforms
	SDL_SetRenderDrawColor(renderer, 0x7D, 0xC1, 0xF0, 0xFF);
	for (Platform p : scene.getPlatforms()) {
		worldRect = p.getRect();

		if (SDL_IntersectRect(&worldRect, &camera.getRect(), &intersectRect) ==
		    SDL_TRUE) {
			worldToSDL(intersectRect, camera.getRect());
			SDL_RenderFillRect(renderer, &intersectRect);
		}
	}
	
	// Draw player
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	worldRect = player.getRect();
	if (SDL_IntersectRect(&worldRect, &camera.getRect(), &intersectRect) == 
	    SDL_TRUE) {
		worldToSDL(intersectRect, camera.getRect());
		SDL_RenderFillRect(renderer, &intersectRect);
	}
	
	// Push update to the window
	SDL_RenderPresent(renderer);
}

void
Render::updateFps(float fps)
{
	if (fullscreen) {
		cout << fps << " fps" << endl;
	}
	else {
		titleStream.str("");
		titleStream.clear();
			
		titleStream << title << " (" << fps << " fps)";
		SDL_SetWindowTitle(window, titleStream.str().c_str());
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
		Log::instance()->error("Render::loadTexture()", error); 
	}
	else {
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL) {
			error << "(" << path << ") " << SDL_GetError();
			Log::instance()->error("Render::loadTexture()", error);
		}
		
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	
	return newTexture;
}

void
Render::worldToSDL(SDL_Rect& rect, const SDL_Rect& cameraRect)
{
	// Shift the rect so its relative window position matches the Camera's
	rect.x -= cameraRect.x;
	rect.y -= cameraRect.y;
	
	// Transform the rect from window to SDL coordinates
	// (0,0) is bottom left in window, top left in SDL
	rect.y = height - (rect.y + rect.h);
}
