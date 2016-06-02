#ifndef RENDER_HPP
#define RENDER_HPP

#include <sstream>
#include <string>
using namespace std;

#include <SDL.h>

#include "Camera.hpp"
#include "Character.hpp"
#include "Scene.hpp"
#include "Types.hpp"

class Render{
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors/Destructor (defined in Render.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Create a Render system with the given block size in world coordinates
	 */
	Render(uint32_t blockSize);
	
	/**
	 * Clean up any memory allocations made by the Render system
	 */
	~Render();

	////////////////////////////////////////////////////////////////////////////
	// Public mutator functions (defined in Render.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Close the currently open SDL_Window and destroy its associated 
	 *   SDL_Renderer
	 *
	 * Returns true if successful, otherwise false
	 */
	bool closeWindow();
	
	/**
	 * Create an SDL_Window and its associated SDL_Renderer according to the 
	 *   flags in renderConfig
	 * 
	 * Returns true if successful, otherwise false
	 */
	bool createWindow(const char* title, const RenderConfig* renderConfig);
	
	/**
	 * Initializes SDL library (MUST BE CALLED BEFORE ANYTHING ELSE)
	 * 
	 * Returns true if successful, otherwise false
	 */
	bool init();
	
	/**
	 * Iterates through the objects in the scene
	 * If they are inside the Camera's view, they're rendered
	 * The player is renderered last
	 */
	void render(const Scene& scene, 
	            const Character& player, 
	            const Camera& camera);
				
	/**
	 * If the game is fullscreen, prints a message to the console with the fps
	 * Otherwise, updates the window title with the current fps
	 */
	void updateFps(float fps);

	////////////////////////////////////////////////////////////////////////////
	// Public accessor functions (defined here, not in Render.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	int getHeight() const {
		return height;
	}

	int getWidth() const {
		return width;
	}

private:
	////////////////////////////////////////////////////////////////////////////
	// Private helper functions (defined in Render.cpp)
	////////////////////////////////////////////////////////////////////////////
	
	/**
	 * Load an SDL_Texture from the specified path.
	 */
	SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
	
	/**
	 * Transform the given rect from world coordinates to SDL coordinates
	 */
	void worldToSDL(SDL_Rect& rect, const SDL_Rect& cameraRect);

	////////////////////////////////////////////////////////////////////////////
	// Private member variables
	////////////////////////////////////////////////////////////////////////////

	bool          fullscreen;  // Whether or not the window is fullscreen
	int           height;      // Height of the window (if not fullscreen)
	int           width;       // Width of the window (if not fullscreen)
	SDL_Renderer* renderer;    // Pointer to the renderer
	SDL_Window*   window;      // Pointer to the window
	string        title;       // Window title
	stringstream  titleStream; // Stream for updating the window title
	uint32_t      blockSize;   // Block size in world coordinates
};

#endif

