#ifndef SLOPE_H
#define SLOPE_H

#include "globals.h"
#include "rectangle.h"
#include <cmath>

class Slope {
public:
	Slope() {}
	Slope(Vector2 p1, Vector2 p2) :
		_p1(p1),
		_p2(p2)
	{
		if (this->_p2.x - this->_p1.x != 0) {
			this->_slope = (fabs(this->_p2.y) - fabs(this->_p1.y)) / (fabs(this->_p2.x) - fabs(this->_p1.x));
		}
	}

	const inline float getSlope() const {
		return this->_slope;
	}

	//implement checking of corner collisions
	const bool collidesWith(const Rectangle &other) {
		if ((other.getRight() >= this->_p2.x &&
			other.getLeft() <= this->_p1.x &&
			other.getTop() <= this->_p2.y &&
			other.getBottom() >= this->_p1.y) ||
			(other.getRight() >= this->_p1.x &&
				other.getLeft() <= this->_p2.x &&
				other.getTop() <= this->_p1.y &&
				other.getBottom() >= this->_p2.y)) {
			if (isInsideSlope(other.getBottom(), other.getRight())) {
				return true;
			}
		}
			 
		else if ((other.getLeft() <= this->_p1.x &&
			other.getRight() >= this->_p2.x &&
			other.getTop() <= this->_p1.y &&
			other.getBottom() >= this->_p2.y) ||
			(other.getLeft() <= this->_p2.x &&
				other.getRight() >= this->_p1.x &&
				other.getTop() <= this->_p2.y &&
				other.getBottom() >= this->_p1.y)) {
			if (isInsideSlope(other.getBottom(), other.getLeft())) {
				return true;
			}
		}
		return false;
	}

	bool isInsideSlope(int rectY, int rectX) {
		//y = mx + b
		float m = _slope;
		int b = (_p1.y - (_slope * fabs(_p1.x)));

		float slopeY = m * rectX + b;
		if (rectY >= slopeY) {
			return true;
		}
		else 
			return false;
	}

	inline Vector2 getP1() { return this->_p1; }
	inline Vector2 getP2() { return this->_p2; }

	Vector2 _p1, _p2;
private:
	
	float _slope;
};

#endif