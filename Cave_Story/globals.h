#ifndef GLOBALS_H
#define GLOBALS_H

namespace globals {
	const float SCREEN_WIDTH = 640;
	const float SCREEN_HEIGHT = 480;
	const float SPRITE_SCALE = 2.0f;
}

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}



enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	INSIDE
};

enum Surface {
	NOTHING,
	RECTANGLE,
	SLOPE
};


struct Vector2 {
	void setX(float x) { this->x = x; };
	void setY(float y) { this->y = y; };
	float getX() { return x; };
	float getY() { return y; };
	float x, y;
	Vector2() :
		x(0), y(0)
	{}
	Vector2(float x, float y) :
		x(x), y(y)
	{}
	Vector2 zero() {
		return Vector2(0, 0);
	}
};

#endif