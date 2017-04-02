#ifndef ANIMATEDTILE_H
#define ANIMATEDTILE_H
#include "globals.h"
#include "tile.h"
#include <vector>

class AnimatedTile : public Tile {
public:
	AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 position);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
protected:
	double _timeElapsed = 0;
	bool _notDone = false;
private:
	//Positions of each of the frames within our tileset
	std::vector<Vector2> _tilesetPositions;
	int _tileFrameIndex;
	int _duration;
};

struct AnimatedTileInfo {
public:
	int TilesetsFirstGid;
	int StartTileId;
	std::vector<int> TileIds;
	int Duration;
};

#endif // !ANIMATEDTILE_H