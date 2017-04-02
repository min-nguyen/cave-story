/*Graphics class*/
/*Holds all information for dealing with game's graphics*/

#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "globals.h"
#include "camera.h"

Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer); 
	SDL_SetWindowTitle(this->_window, "CaveStory");
	SDL_SetRenderDrawColor(this->_renderer, 255, 255, 255, 255);
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image couldnt be initialized");
	}
}

Graphics::~Graphics() {
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
	
}

SDL_Surface* Graphics::loadSurface(const std::string &filePath) {
	printf("\nTrying to load surface: %s\n", filePath.c_str());
	if (this->_spriteSheets.count(filePath) == 0) {
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	if (this->_spriteSheets[filePath] == NULL) {
		printf("\n Entry in spritesheets map is still null, IMG_Load error: %s\n", IMG_GetError());
	}
	else {
		printf("\nSuccessfully loaded: %s\n", filePath.c_str());
	}
	return this->_spriteSheets[filePath];
}

SDL_Texture* Graphics::loadTexture(SDL_Surface* surface) {
	SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);

	if (texture == NULL) {
		printf("\nUnable to create texture from surface! SDL_image error : %s\n", SDL_GetError());
	}
	else
		return texture;
}

void Graphics::clear() {
	SDL_RenderClear(this->_renderer);
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destRectangle){
	destRectangle->x -= Camera::camera.x;
	destRectangle->y -= Camera::camera.y;
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destRectangle);
}

void Graphics::flip() {
	SDL_RenderPresent(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const{
	return this->_renderer;
}