#include "hud.h"
#include "camera.h"
#include "graphics.h"
#include "player.h"

HUD::HUD(){}

HUD::HUD(Graphics &graphics, Player *player) :
	_player(player),
	_healthBarSprite(Sprite(graphics, "sprites/textbox.png", 0, 40, 64, 8, 35, 70)),
	_healthNumber1(Sprite(graphics, "sprites/textbox.png", 0, 56, 8, 8, 66, 70)),
	_currentHealthBar(Sprite(graphics, "sprites/textbox.png", 0, 25, 39, 5, 83, 72)),
	_lvlNumber(Sprite(graphics, "sprites/textbox.png", 0, 56, 8, 8, 66, 52)),
	_lvlWord(Sprite(graphics, "sprites/textbox.png", 81, 81, 11, 7, 38, 52)),
	_expBar(Sprite(graphics, "sprites/textbox.png", 0, 72, 40, 8, 83, 52)),
	_slash(Sprite(graphics, "sprites/textbox.png", 72, 48, 8, 8, 100, 36)),
	_topDash(Sprite(graphics, "sprites/textbox.png", 81, 51, 15, 4, 132, 26)),
	_bottomDash(Sprite(graphics, "sprites/textbox.png", 81, 51, 15, 4, 132, 42))
{}

void HUD::update(int elapsedTime) {
	_healthNumber1.setSourceRectX(_player->getCurrentHealth() * 8);
	_currentHealthBar.setSourceRectW(_player->getCurrentHealth()*13);
}

void HUD::draw(Graphics& graphics) {
	_healthBarSprite.draw(graphics, _healthBarSprite.getX() + Camera::camera.x, _healthBarSprite.getY() + Camera::camera.y);
	_healthNumber1.draw(graphics, _healthNumber1.getX() + Camera::camera.x, _healthNumber1.getY() + Camera::camera.y);
	_currentHealthBar.draw(graphics, _currentHealthBar.getX() + Camera::camera.x, _currentHealthBar.getY() + Camera::camera.y);
	_lvlWord.draw(graphics, _lvlWord.getX() + Camera::camera.x, _lvlWord.getY() + Camera::camera.y);
	_lvlNumber.draw(graphics, _lvlNumber.getX() + Camera::camera.x, _lvlNumber.getY() + Camera::camera.y);
	_expBar.draw(graphics, _expBar.getX() + Camera::camera.x, _expBar.getY() + Camera::camera.y);
	_slash.draw(graphics, _slash.getX() + Camera::camera.x, _slash.getY() + Camera::camera.y);
	_topDash.draw(graphics, _topDash.getX() + Camera::camera.x, _topDash.getY() + Camera::camera.y);
	_bottomDash.draw(graphics, _bottomDash.getX() + Camera::camera.x, _bottomDash.getY() + Camera::camera.y);
}