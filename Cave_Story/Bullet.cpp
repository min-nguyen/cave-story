#include "bullet.h"
#include "weapon.h"
Bullet::Bullet() {};

Bullet::Bullet(Graphics &graphics, const std::string &filePath, int srcX, int srcY,
	float width, float height, float timeToUpdate, Weapon* weapon)
	: AnimatedSprite(graphics, filePath, srcX, srcY, width, height,
		weapon->getX() + 20, weapon->getY(), timeToUpdate), _weapon(weapon) {
}

void Bullet::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, _x, _y);
}

void Bullet::update(float elapsedTime, Level& level) {
	_existTime += elapsedTime;
	if (_existTime > 1000)
		_weapon->remove(this);
	AnimatedSprite::update(elapsedTime);
}

BulletA::BulletA() {}

BulletA::BulletA(Graphics &graphics, Weapon* weapon) :
	Bullet(graphics, "sprites/bullet.png", 0, 0, 14, 14, 120, weapon)
{
	setupAnimations();
	playAnimation("Right");
}

void BulletA::setupAnimations() {
	///RIGHT
	SDL_Rect r1 = { 128, 0, 14, 14 };
	SDL_Rect r2 = { 146, 0, 14, 14 };
	SDL_Rect r3 = { 162, 0, 14, 14 };
	SDL_Rect r4 = { 178, 0, 14, 14 };
	std::vector<SDL_Rect> rightList;
	rightList.push_back(r1); rightList.push_back(r2); rightList.push_back(r3); rightList.push_back(r4);
	addAnimationV2(rightList, "Right", Vector2(0, 0));
}

void BulletA::draw(Graphics& graphics) {
	Bullet::draw(graphics);
}

void BulletA::update(float elapsedTime, Level& level) {

	_x += _dx*elapsedTime;
	_y += _dy*elapsedTime;
	Bullet::update(elapsedTime, level);
}

void BulletA::animationDone(std::string currentAnimation) {
}

void BulletA::handleTileCollision(std::vector<Rectangle> &others) {

}

