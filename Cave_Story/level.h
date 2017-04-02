#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include "slope.h"
#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "animatedtile.h"
#include "door.h"
#include "bullet.h"
class Weapon;
class Enemy;
class Player;
class Graphics;
class Bullet;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	Level(std::string mapName, Graphics &graphics);
	~Level();
	void update(float elapsedTime, Player &player);
	void draw(Graphics& graphics);
	std::vector<Rectangle> checkTileCollisions(const Rectangle& other);
	std::vector<Slope> checkSlopeCollisions(const Rectangle& other);
	std::vector<Door> checkDoorCollisions(const Rectangle& other);
	std::vector<Enemy*> checkEnemyCollisions(const Rectangle& other);
	std::vector<Weapon*> checkWeaponCollisions(const Rectangle& playerRect);
	const Vector2 getPlayerSpawnPoint() const;
	void removeEnemy(Enemy* enemy);
	std::vector<Tile> _tileList; //
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;//
	std::vector<Slope> _slopes; //
	std::vector<AnimatedTile> _animatedTileList; //
	std::vector<AnimatedTileInfo> _animatedTileInfos;
	std::vector<Door> _doorList; //
	std::vector<Enemy*> _enemies;
	std::vector<Weapon*> _weapons;
	std::vector<Bullet*> _bullets;
	Vector2 getSize() { return _size; };
private:
	std::string _mapName;
//	SDL_Texture* _backgroundTexture;
	Vector2 _spawnPoint;
	Vector2 _size;
	Vector2 _tileSize;
	

	void loadMap(std::string mapName, Graphics& graphics);

	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
};




//Tileset structure
struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;
	Tileset() {
		this->FirstGid = -1;
	}
	Tileset(SDL_Texture* texture, int firstGid) {
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};




#endif