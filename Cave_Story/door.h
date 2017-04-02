#ifndef DOOR_H
#define DOOR_H

#include <string>
#include "rectangle.h"
#include "globals.h"
#include "graphics.h"

class Door : public Rectangle{
public:
	Door() {};
	Door(Rectangle r, std::string destination) :
		Rectangle(r.getLeft() * globals::SPRITE_SCALE, r.getTop() * globals::SPRITE_SCALE,
			r.getWidth() * globals::SPRITE_SCALE, r.getHeight() * globals::SPRITE_SCALE),
		_destination(destination)
	{};
	const inline std::string getDestination() const { return _destination; };
	
private:
	std::string _destination;
	float _x, _y;
};

#endif
