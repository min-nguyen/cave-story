/*Game Class*/
/*Holds all information concerning main game loop*/

#ifndef GAME_H
#define GAME_H

#include "level.h"
#include "hud.h"
#include "graphics.h"

class Player;

class Game {
public:
	Game();
	~Game();

private:
	/*Begins game and gameloop*/
	void gameLoop();
	/*Calls Sprite object's draw, which calls Graphic's blitSurface, which calls RenderCopy*/
	void draw(Graphics &graphics);
	/*Calls updates from all necessary data*/
	void update(float elapsedTime, Graphics &graphics);
	/*Main player*/
	Player _player;
	/*Level*/
	Level _level;
	HUD _hud;
	Graphics _graphics;
	Graphics _graphics2;
	Rectangle _camera;
};
#endif