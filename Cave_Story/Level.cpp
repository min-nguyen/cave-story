#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "player.h"
#include "enemy.h"
#include "tinyxml2.h"
#include "slope.h"
#include <SDL.h>
#include "utils.h"
#include <sstream>
#include <algorithm>
#include <cmath>
#include "weapon.h"

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Graphics &graphics) :
	_mapName(mapName),
	_size(Vector2(0, 0))
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {

}

/*NOTE*/
/*Gid is the number of the tile from the source tileset, taking into account which tileset we are using.
For example: Tileset1's first tile would have a gid of 1. If there are 50 tiles in Tileset1, then Tileset2's first
tile would have a gid of 52. Therefore we have to take this into account (especially during getTilesetPosition).
Thus we have a member "firstGid" for Tileset structs, to indicate what is the gid of the first tile from the tileset.

Tileid is for animated tiles, and each one is the same as a gid - representing the gid of the tile taken from the tileset.
However, it does not take into account which tileset we are on. So the first gid of Tileset2 would have a tileid of 1, whereas
the gid number of that tile is actually 52. Therefore we have to take this into account when declaring the field startTileId
for AnimatedTileInfo structs.
*/

void Level::loadMap(std::string mapName, Graphics &graphics) {

	

	//Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "maps/" << mapName << ".tmx"; //Pass in Map 1, we get maps/Map 1.tmx
	doc.LoadFile(ss.str().c_str());
	XMLElement* mapNode = doc.FirstChildElement("map");

	//Get the width and the height of the whole map and store it in _size
	int mapWidth, mapHeight;
	mapNode->QueryIntAttribute("width", &mapWidth);
	mapNode->QueryIntAttribute("height", &mapHeight);
	this->_size = Vector2(mapWidth, mapHeight);

	//Get the width and the height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	/*Creating _tilesets*/
	//Loading all the tilesets and adding them to _tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			//Convert to string, and deletes first 3 unnecessary characters "../" which cause an error
			std::string filePath = source;
			filePath.erase(0, 3);
			std::stringstream ss;
			ss << filePath;
			pTileset->QueryIntAttribute("firstgid", &firstgid);

			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadSurface(ss.str()));

			this->_tilesets.push_back(Tileset(tex, firstgid));

			//Get all the animations for that tileset
			XMLElement* pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL) {
				while (pTileA) {
					AnimatedTileInfo ati;
					ati.StartTileId = pTileA->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;
					XMLElement* pAnimation = pTileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						while (pAnimation) {
							XMLElement* pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}

							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					_animatedTileInfos.push_back(ati);

					pTileA = pTileA->NextSiblingElement("tile");
				}
			}


			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	/*Loading the layers*/
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			//Loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					//Loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//Build each individual tile here
							//If gid is 0, no tile should be drawn. Continue loop
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									if (this->_tilesets[i].FirstGid > closest) {
										closest = this->_tilesets[i].FirstGid;
										tls = this->_tilesets.at(i);
									}
								}
							}

							if (tls.FirstGid == -1) {
								//No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}

							//Get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % mapWidth;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / mapWidth);
							Vector2 finalTilePosition = Vector2(xx, yy);

							//Calculate the position of the tile in the tileset
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							/*Build the actual tile and add it to the level's tile list*/
							/***********************************************************/

							//Check if the current tile (current gid we're on) is an animated tile
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < this->_animatedTileInfos.size(); i++) {
								//If it's an animated tile, then the gid number will be the same as the first animation frame's gid
								if (this->_animatedTileInfos.at(i).StartTileId == gid) {
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							//If animated, created an animated tile, and add to animatedTileList
							if (isAnimatedTile == true) {
								//Get all positions of the animated tile frames from the source tileset, so we know where to extract from
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i),
										tileWidth, tileHeight));
								}
								//Create our new animated tile using this info, and add to animatedTileList
								AnimatedTile tile(tilesetPositions, ati.Duration,
									tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
								this->_animatedTileList.push_back(tile);
							}
							//If non-animated, create normal tile, and add to tileList
							else {
								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight),
									finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}

							tileCounter++;
							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}

			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	//Loading and parsing the objects
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			//Parse out collisions
			if (ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "slopes") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));

						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							//Get each (x,y) pair
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');
							//Now we have each of the pairs. Loop through list and split into Vector2s.

							for (int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								//x will be held at index 0, y will be held at index 1
								Vector2 p2 = Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1)));
								points.push_back(p2);

							}
						}

						for (int i = 0; i < points.size() - 1; i++) {
							Vector2 point1 = Vector2((p1.x + points.at(i).x) * globals::SPRITE_SCALE,
								(p1.y + points.at(i).y) * globals::SPRITE_SCALE);
							Vector2 point2 = Vector2((p1.x + points.at(i + 1).x) * globals::SPRITE_SCALE,
								(p1.y + points.at(i + 1).y) * globals::SPRITE_SCALE);
							Slope slope = Slope(point1, point2);

							_slopes.push_back(slope);
							printf("\n Slope for my method - P1: %d, %d P2: %d, %d\n", slope.getP1().x, slope.getP1().y, slope.getP2().x, slope.getP2().y);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			//Parse out spawn point
			else if (ss.str() == "spawn points") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE,
								std::ceil(y) * globals::SPRITE_SCALE);
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			//Parse out doors
			else if (ss.str() == "doors") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle doorRect = Rectangle(x, y, w, h);

						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL) {
							while (pProperties != NULL) {
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL) {
									while (pProperty != NULL) {
										std::stringstream ss;
										const char* name = pProperty->Attribute("name");
										ss << name;
										if (ss.str() == "destination") {
											std::stringstream ss2;
											const char* value = pProperty->Attribute("value");
											ss2 << value;
											Door door = Door(doorRect, ss2.str());
											_doorList.push_back(door);
										}

										pProperty = pProperty->NextSiblingElement("property");
									}
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "enemies") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "bat") {
							this->_enemies.push_back(new Bat(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
								std::floor(y) * globals::SPRITE_SCALE)));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "weapons") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "gun") {
							this->_weapons.push_back(new Gun(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
								std::floor(y) * globals::SPRITE_SCALE)));
						}

					pObject = pObject->NextSiblingElement("object");
					}
				}
			}
		

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

void Level::removeEnemy(Enemy* enemy) {
	_enemies.erase(std::remove(_enemies.begin(), _enemies.end(), enemy), _enemies.end());
	delete(enemy);
}

void Level::update(float elapsedTime, Player &player) {
	/**Updates Animated Tiles**/
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		_animatedTileList.at(i).update(elapsedTime);
	}
	/**Updates enemies**/
	for (int i = 0; i < _enemies.size(); i++) {
		_enemies.at(i)->update(elapsedTime, player, *this);
	}
	/**Updates weapons**/
	for (int i = 0; i < _weapons.size(); i++) {
		_weapons.at(i)->update(elapsedTime, *this);
	}

}

void Level::draw(Graphics &graphics) {
	for (int i = 0; i < this->_tileList.size(); i++) {
		_tileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		_animatedTileList.at(i).draw(graphics);
	}
	for (int i = 0; i < _enemies.size(); i++) {
		_enemies.at(i)->draw(graphics);
	}
	for (int i = 0; i < _weapons.size(); i++) {
		if(!_weapons.at(i)->_isHeld)
			_weapons.at(i)->draw(graphics);
	}

}

//Takes level's collision tiles - checks they've collided with a given rect
std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
	std::vector<Slope> others;
	for (int i = 0; i < _slopes.size(); i++) {
		if (_slopes.at(i).collidesWith(other)) {
			others.push_back(_slopes.at(i));
		}
	}
	return others;
}

std::vector<Door> Level::checkDoorCollisions(const Rectangle &other) {
	std::vector<Door> others;
	for (int i = 0; i < _doorList.size(); i++) {
		if (_doorList.at(i).collidesWith(other)) {
			others.push_back(_doorList.at(i));
		}
	}
	return others;
}

std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle& playerRect) {
	std::vector<Enemy*> others;
	for (int i = 0; i < _enemies.size(); i++) {
		if (_enemies.at(i)->getBoundingBox().collidesWith(playerRect)) {
			others.push_back(_enemies.at(i));
		}
	}
	return others;
}

std::vector<Weapon*> Level::checkWeaponCollisions(const Rectangle& playerRect) {
	std::vector<Weapon*> others;
	for (int i = 0; i < _weapons.size(); i++) {
		if (_weapons.at(i)->getBoundingBox().collidesWith(playerRect)) {
			others.push_back(_weapons.at(i));
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	//Calculate the position of the tile in the tileset
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = (gid - 1) % (tilesetWidth / tileWidth);
	tsxx *= tileWidth;

	int amt = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	int tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

	return finalTilesetPosition;
}