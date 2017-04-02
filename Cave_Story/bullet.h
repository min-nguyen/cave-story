#ifndef BULLET_H
#define BULLET_H

#include "animatedsprite.h"
#include "level.h"
#include "weapon.h"
class Graphics;

class Bullet : public AnimatedSprite {
public:
	Bullet();
	Bullet(Graphics &graphics, const std::string &filePath, int srcX, int srcY, float width, float height, float timeToUpdate, Weapon* weapon);
	virtual void draw(Graphics& graphics);
	virtual void update(float elapsedTime, Level& level);
	//virtual void handleTileCollision(std::vector<Rectangle> &others) = 0;
	float _existTime = 0;
	float _dx = 0;
	float _dy = 0;
	Weapon* _weapon;
};

class BulletA : public Bullet {
public:
	BulletA();
	BulletA(Graphics &graphics, Weapon* weapon);
	void draw(Graphics& graphics);
	void update(float elapsedTime, Level& level);
	void animationDone(std::string currentAnimation);
	void handleTileCollision(std::vector<Rectangle> &others);
private:
	void setupAnimations();
};



#endif


