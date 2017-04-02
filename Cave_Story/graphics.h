#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <map>
#include <string>
#include <SDL.h>

/*Graphics class*/
/*Holds all information for dealing with game's graphics*/

struct SDL_Window;
struct SDL_Renderer;

class Graphics {
public:
	Graphics();
	~Graphics();

	/*SDL_Surface* loadSurface
	  Loads an image surface into _spriteSheets map if it doesn't already exist
	  As a result, each image will only ever be loaded once
	  Returns the image from the map regardless of whether or not it was just loaded
	*/
	SDL_Surface* loadSurface(const std::string &filePath);

	/*SDL_Texture* loadTexture
	  Converts surface to texture
	*/
	SDL_Texture* Graphics::loadTexture(SDL_Surface* surface);

	/*void blitSurface - RenderCopy
	  Draws a texture to a certain part of the screen - same as SDL_blitSurface but with textures
	*/
	void blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destRectangle);

	/*void flip - RenderPresent
	  Renders current renderer everything to the screen
	*/
	void flip();

	/*void clear - RenderClear
	  Clears the renderer screen
	*/
	void clear();

	/*SDL_Renderer* getRenderer
	  Returns the renderer
	*/
	SDL_Renderer* getRenderer() const;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	SDL_Texture* texture;
private:
	
	
	//Contains all sprite sheets
	std::map<std::string, SDL_Surface*> _spriteSheets;
};


#endif