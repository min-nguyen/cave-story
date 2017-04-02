/*Tile Class
Contains data and logic for an individual tile*/

#ifndef TILE_H
#define TILE_H
#include "rectangle.h"
#include "globals.h"

struct SDL_Texture;
class Graphics;

class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);
	void update(int elapsedTime);
	void draw(Graphics &graphics);
	void setX(float x) { _position.x = x; };
	void setY(float y) { _position.y = y; };
	float getX() { return _position.x; };
	float getY() { return _position.y; };
protected:
	/*Size - Dimensions of tile*/
	Vector2 _size;
	/*TilesetPosition - Position in tileset that the tile comes from*/
	Vector2 _tilesetPosition;
	/*Position - Position on the map that the tile is on*/
	Vector2 _position;
	/*Tileset - Texture/image that the tile is taken from*/
	SDL_Texture* _tileset;
private:
	
	
};

#endif