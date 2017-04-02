#ifndef CAMERA_H
#define CAMERA_H

struct SDL_Rect;
class Player;

class Camera {
public:
	static SDL_Rect camera;
	static void update(float elapsedTime, Player& _player);
	static SDL_Rect getCamera();
};
#endif