#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

#include <string>

#include "rectangle.h"
#include "globals.h"

class Graphics;

/* Sprite class
* Holds all information for individual sprites
*/

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);
	virtual ~Sprite();
	virtual void update();
	virtual void draw(Graphics &graphics, int x, int y);
	void setX(float x) { _x = x; };
	void setY(float y) { _y = y; };
	float getX() { return _x; };
	float getY() { return _y; };
	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle other) const;
	void setSourceRectX(int x) { _sourceRect.x = x; };
	void setSourceRectY(int y) { _sourceRect.y = y; };
	void setSourceRectH(int h) { _sourceRect.h = h; };
	void setSourceRectW(int w) { _sourceRect.w = w; };
protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;

	Rectangle _boundingBox;

	float _x, _y;
private:
};


#endif