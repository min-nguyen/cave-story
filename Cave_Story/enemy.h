#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"
#include "globals.h"
#include "player.h"

#include <string>

class Graphics;

class Enemy : public AnimatedSprite{
public:
	Enemy();
	Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
		int width, int height, Vector2 spawnPoint, int timeToUpdate);
	virtual void draw(Graphics &graphics);
	virtual void update(float elapsedTime, Player& player, Level& level);
	virtual void updateCollisions(Level& level) = 0;
	virtual void handleTileCollision(std::vector<Rectangle> &others) = 0;
	virtual void handlePlayerCollision(Level& level, Player& player, sides::Side collisionSide, Rectangle playerRect) = 0;
	int getMaxHealth() { return _maxHealth; };
	int getCurrentHealth() { return _currentHealth; };
	virtual void attackPlayer(Player& player) = 0;

protected:
	Direction _direction;
	int _maxHealth, _currentHealth;
	Vector2 _spawnPoint;
};

class Bat : public Enemy {
public:
	Bat();
	Bat(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime, Player& player, Level& level);
	void updateCollisions(Level& level);
	void handleTileCollision(std::vector<Rectangle> &others);
	void handlePlayerCollision(Level& level, Player& player, sides::Side collisionSide, Rectangle playerRect);
	void attackPlayer(Player& player);
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	
private:
	Direction _movingDirection = LEFT;
	float _dx, _dy;
	float _distanceTravelled = 0; //Used as replacement for _x
};

#endif