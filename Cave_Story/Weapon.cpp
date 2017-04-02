#include "level.h"
#include "weapon.h"
#include "bullet.h"
#include <vector>

using namespace std;

namespace weapon_constants {
	const float GRAVITY = 0.012f;
}

Weapon::Weapon() {}

Weapon::Weapon(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, Vector2 spawnPoint, float width, float height, float timeToUpdate)
	: AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
		spawnPoint.x, spawnPoint.y, timeToUpdate), _spawnPoint(spawnPoint)
{

}

void Weapon::draw(Graphics& graphics) {
	for (int i = 0; i < _ammo.size(); i++) {
		_ammo.at(i)->draw(graphics);
	}
	AnimatedSprite::draw(graphics, _x, _y);
}

void Weapon::update(float elapsedTime, Level& level) {
	for (int i = 0; i < _ammo.size(); i++) {
		_ammo.at(i)->update(elapsedTime, level);
	}
	AnimatedSprite::update(elapsedTime);
}

void Weapon::remove(Bullet* bullet) {
	for (int i = 0; i < _ammo.size(); i++) {
		if (_ammo.at(i) == bullet) {
			_ammo.erase(_ammo.begin() + i);
			delete(bullet);
		}
	}
}
Gun::Gun() {}

Gun::Gun(Graphics &graphics, Vector2 spawnPoint)
	: Weapon(graphics, "sprites/arms.png", 98, 9, spawnPoint, 20, 12, 120)
{
	setupAnimations();
	playAnimation("Right");

}

void Gun::shoot(Graphics &graphics) {
	printf("current animation = %s\n", _currentAnimation.c_str());
	BulletA* bullet = new BulletA(graphics, this);
	printf("current animation %s\n", _currentAnimation.c_str());
	if (strcmp(_currentAnimation.c_str(), "Right") == 0) {
		bullet->_dx = _bulletSpeed;
	}
	if (strcmp(_currentAnimation.c_str(), "Left") == 0)
		bullet->_dx = -_bulletSpeed;
	if (strcmp(_currentAnimation.c_str(), "UpLeft") == 0)
		bullet->_dy = -_bulletSpeed;
	if (strcmp(_currentAnimation.c_str(), "UpRight") == 0)
		bullet->_dy = -_bulletSpeed;
	_ammo.push_back(bullet);
}

void Gun::setupAnimations() {
	addAnimation(1, 4, 5, "Left", 24.5, 10, Vector2(0, 0));
	addAnimation(1, 4, 20, "Right", 24.5, 10, Vector2(0, 0));
	addAnimation(1, 4, 30, "UpLeft", 24, 10, Vector2(0, 0));
	addAnimation(1, 4, 46, "UpRight", 24.5, 10, Vector2(0, 0));
}

void Gun::update(float elapsedTime, Level& level) {
	if (!(_isHeld)) {
		//CHECK LEVEL COLLISIONS
		std::vector<Rectangle> collidingTilesForWeapon = level.checkTileCollisions(getBoundingBox());
		if (collidingTilesForWeapon.size() > 0)
			handleTileCollision(collidingTilesForWeapon);

		//APPLY GRAVITY
		this->_dy += weapon_constants::GRAVITY * elapsedTime;
		this->_y += this->_dy * elapsedTime;
	}
	Weapon::update(elapsedTime, level);
}

void Gun::handleTileCollision(std::vector<Rectangle> &others) {
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				_dy = 0;
				this->_y = others.at(i).getBottom() + 1;
				break;
			case sides::BOTTOM:
				_dy = 0;
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			}
		}
	}
}

void Gun::draw(Graphics& graphics) {
	Weapon::draw(graphics);
}

void Gun::animationDone(std::string currentAnimation) {

}