#include "enemy.h"
#include "sprite.h"
#include "globals.h"

//Base enemy class
Enemy::Enemy() {}

Enemy::Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
	Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
		spawnPoint.x, spawnPoint.y, timeToUpdate),
	_spawnPoint(spawnPoint),
	_direction(LEFT),
	_maxHealth(0),
	_currentHealth(0)
{}

void Enemy::update(float elapsedTime, Player &player, Level& level) {
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}


//Bat class

namespace BAT_CONSTANTS {
	float FLY_SPEED = 0.04f;
	float RANGE = 50;
}

Bat::Bat() {}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "sprites/npc/npccemet.png", 32, 32, 16, 16, spawnPoint, 120),
	_dx(0), _dy(0), _movingDirection(LEFT)
{
	this->setupAnimations();
	this->playAnimation("FlyLeft");
}

void Bat::updateCollisions(Level& level) {
	//Handle tile collisions on level
	std::vector<Rectangle> _collidingTilesForEnemy = level.checkTileCollisions(getBoundingBox());
	if (_collidingTilesForEnemy.size() > 0) {
		printf("collide\n");
		for (int j = 0; j < _collidingTilesForEnemy.size(); j++) {
			handleTileCollision(_collidingTilesForEnemy);
		}
	}
}

void Bat::update(float elapsedTime, Player &player, Level& level) {
	
	//Update left and right movement 
	if (_movingDirection == LEFT ) {
		this->_x -= BAT_CONSTANTS::FLY_SPEED;
		_distanceTravelled -= BAT_CONSTANTS::FLY_SPEED;
		if (_distanceTravelled < - BAT_CONSTANTS::RANGE) {
			_movingDirection = RIGHT;
		}
	}
	else if (_movingDirection == RIGHT){
		this->_x += BAT_CONSTANTS::FLY_SPEED;
		_distanceTravelled += BAT_CONSTANTS::FLY_SPEED;
		if (_distanceTravelled < -BAT_CONSTANTS::RANGE) {
			_movingDirection = LEFT;
		}
	}
	//Update animation depending on direction faced
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");
	Enemy::update(elapsedTime, player, level);
}

void Bat::handlePlayerCollision(Level &level, Player& player, sides::Side collisionSide, Rectangle playerRect) {
	switch (collisionSide) {
	case sides::BOTTOM:
		level.removeEnemy(this);
		player.bounce();
		break;
	case sides::TOP:
		this->attackPlayer(player);
		break;
	case sides::LEFT:
		this->attackPlayer(player);
		break;
	case sides::RIGHT:
		this->attackPlayer(player);
		break;
	}
	
}

void Bat::attackPlayer(Player& player) {
	player.gainHealth(-1);
}

void Bat::handleTileCollision(std::vector<Rectangle> &others) {
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		switch (collisionSide) {
		case sides::LEFT:
			_movingDirection = RIGHT;
			break;
		case sides::RIGHT:
			_movingDirection = LEFT;
			break;
		default:
			break;
		}
	}
}

void Bat::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Bat::animationDone(std::string currentAnimation) {

}

void Bat::setupAnimations() {
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
}