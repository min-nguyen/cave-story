#ifndef WEAPON_H
#define WEAPON_H
#include "globals.h"
#include "graphics.h"
#include "animatedsprite.h"
class Bullet;
class Level;

class Weapon : public AnimatedSprite {
public:
	Weapon();
	Weapon(Graphics &graphics, const std::string &filePath, int srcX, int srcY, Vector2 spawnPoint, float width, float height, float timeToUpdate);
	virtual void draw(Graphics& graphics);
	virtual void update(float elapsedTime, Level& level);
	virtual void handleTileCollision(std::vector<Rectangle> &others) = 0;
	virtual void shoot(Graphics& graphics) = 0;
	bool _isHeld = false;
	std::vector<Bullet*> _ammo;
	void remove(Bullet* bullet);
protected:
	virtual void setupAnimations() = 0;
	Vector2 _spawnPoint;
	float _dy = 0;

};

class Gun : public Weapon {
public:
	Gun();
	Gun(Graphics &graphics, Vector2 spawnPoint);
	void shoot(Graphics& graphics);
	void draw(Graphics& graphics);
	void update(float elapsedTime, Level& level);
	void animationDone(std::string currentAnimation);
	void handleTileCollision(std::vector<Rectangle> &others);
private:
	const float _bulletSpeed = 0.2f;
	void setupAnimations();
};
#endif