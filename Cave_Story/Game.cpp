#include <SDL.h>

#include "game.h"
#include "graphics.h"
#include "input.h"
#include <algorithm>
#include "level.h"
#include "enemy.h"
#include "weapon.h"
#include "camera.h"
/* Game class
* This class holds all information for our main game loop
*/

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop();
}

Game::~Game() {

}

void Game::gameLoop() {


	Graphics graphics;
	_graphics = graphics;

	Input input;
	SDL_Event event;
	
	this->_level = Level("map3", graphics);
	this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
	Player* playerPointer = &_player;
	
	this->_hud = HUD(graphics, playerPointer);


	int LAST_UPDATE_TIME = SDL_GetTicks();
	//Start the game loop
	while (true) {
		input.beginNewFrame();
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			return;
		}
		else if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
			this->_player.moveLeft();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
			this->_player.moveRight();
		}
		if (input.isKeyHeld(SDL_SCANCODE_UP) == true) {
			this->_player.lookUp();
		}
		if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true) {
			this->_player.lookDown();
		}
		if (!input.isKeyHeld(SDL_SCANCODE_UP) && !input.isKeyHeld(SDL_SCANCODE_DOWN)) {
			this->_player.lookForward();
		}
		if (input.wasKeyPressed(SDL_SCANCODE_A) == true) {
			this->_player.jump();
		}
		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
			this->_player.stopMoving();
		}
		if (input.wasKeyPressed(SDL_SCANCODE_D)) {
			_player.shoot(graphics);
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		Camera::update(ELAPSED_TIME_MS, _player);
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
		
	}
}

void Game::draw(Graphics &graphics) {
	graphics.clear();
	this->_level.draw(graphics);
	this->_player.draw(graphics);
	this->_hud.draw(graphics);
	graphics.flip();

}

void Game::update(float elapsedTime, Graphics& graphics) {

	this->_player.update(elapsedTime, _level, *this, graphics);
	this->_level.update(elapsedTime, _player);
	this->_hud.update(elapsedTime);

}
