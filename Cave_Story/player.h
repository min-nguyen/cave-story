#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "slope.h"
#include "door.h"
#include "level.h"

class Game;
class Graphics;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime, Level& level, Game& game, Graphics& graphics);
	bool atRight = false;
	void shoot(Graphics& graphics);
	void moveLeft();
	void moveRight();
	void stopMoving();
	void jump();
	void bounce();
	void lookUp();
	void lookDown();
	void lookForward();
	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();
	void gainHealth(int x);
	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);
	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Game& game);
	void handleEnemyCollisions(Level& level, std::vector<Enemy*> others);
	void handleWeaponCollisions(Level& level, std::vector<Weapon*> others);
	const float getX() const { return _x; };
	const float getY() const { return _y; };
	const int getCurrentHealth() { return _currentHealth; };
	void updateWeapon(float elapsedTime, Graphics& graphics);
	void updateCollisions(Level& level, Game& game, Graphics& graphics);
	bool _invulnerable = false;
	Direction _cameraHorizontal = INSIDE;
	Direction _cameraVertical = INSIDE;
	float _dx, _dy;
	
private:
	//Rate of change of x and y (dx/dt and dy/dt)
	//State of player
	Direction _facing;
	bool _grounded;
	bool _lookingUp, _lookingDown;
	//Surface Enum : {NOTHING, RECTANGLE, SLOPE}
	Surface _currentSurface = NOTHING;
	Rectangle _lastCollidedFloorRect;
	Rectangle _lastCollidedLeftRect;
	Rectangle _lastCollidedRightRect;
	Slope _lastCollidedSlope;
	//Health attributes
	int _maxHealth, _currentHealth;
	float damageTimer = 0;
	float blipTimer = 0;
	float _runSpeed, _jumpSpeed, _gravity;
	//Weapon
	Weapon* _weapon = NULL;
};

#endif









