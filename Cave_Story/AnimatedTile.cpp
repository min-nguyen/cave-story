#include "animatedtile.h"
#include "graphics.h"

AnimatedTile::AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, 
									SDL_Texture* tileset, Vector2 size, Vector2 position)
	:	Tile(tileset, size, tilesetPositions.at(0), position),
		_tilesetPositions(tilesetPositions),
		_duration(duration),
		_tileFrameIndex(0)
{}

void AnimatedTile::update(int elapsedTime) {
	Tile::update(elapsedTime);

	if (_timeElapsed >= _duration) {
		if (_tileFrameIndex >= _tilesetPositions.size() - 1) {
			_tileFrameIndex = 0;
		}
		else {
			_tileFrameIndex++;
		}
		_timeElapsed = 0;
	}
	else {
		_timeElapsed = _timeElapsed + elapsedTime;
	}
}

void AnimatedTile::draw(Graphics &graphics) {
	SDL_Rect destRect = {_position.x, _position.y, _size.x * globals::SPRITE_SCALE, _size.y * globals::SPRITE_SCALE};
	SDL_Rect sourceRect = {_tilesetPositions.at(_tileFrameIndex).x, _tilesetPositions.at(_tileFrameIndex).y, _size.x, _size.y};

	graphics.blitSurface(_tileset, &sourceRect, &destRect);
}