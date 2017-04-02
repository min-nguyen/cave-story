#ifndef HUD_H
#define HUD_H

#include "sprite.h"
#include "player.h"
class Graphics;


class HUD {
public:
	HUD();
	HUD(Graphics& graphics, Player* player);
	void update(int elapsedTime);
	void draw(Graphics& graphics);
private:
	Player* _player;
	//Health sprites
	Sprite _healthBarSprite;
	Sprite _healthNumber1;
	Sprite _currentHealthBar;
	//Exp sprites
	Sprite _lvlWord;
	Sprite _lvlNumber;
	Sprite _expBar;
	//Weapon info
	Sprite _slash;
	Sprite _topDash;
	Sprite _bottomDash;
};


#endif