#include "camera.h"
#include <SDL.h>
#include <SDL_rect.h>
#include "player.h"

SDL_Rect Camera::camera = { 0, 0, 500, 500 };

void Camera::update(float elapsedTime, Player& player) {
	camera.x = player.getX() - globals::SCREEN_WIDTH/2;
	camera.y = player.getY() - globals::SCREEN_HEIGHT/2;
}

SDL_Rect Camera::getCamera() {
	return camera;
}