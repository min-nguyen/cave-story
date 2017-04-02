#include "player.h"
#include "graphics.h"
#include "slope.h"
#include "enemy.h"
#include "game.h"
#include "weapon.h"
namespace player_constants {
	const float WALK_SPEED = 0.2f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
	const float JUMP = 0.8f;
	const float BOUNCE = 0.5f;
	const float DAMAGE_TIMER = 2000.0f;
	const float DAMAGE_BLIP = 200.0f;
}

Player::Player() {}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "sprites/mychar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false),
	_maxHealth(3),
	_currentHealth(2),
	_runSpeed(player_constants::WALK_SPEED),
	_jumpSpeed(player_constants::JUMP)
{

	this->setupAnimations();
	this->playAnimation("RunRight");
}

void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 0, "LookUpIdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 16, "LookUpIdleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 0, "LookUpRunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 16, "LookUpRunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 0, "LookDownIdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 16, "LookDownIdleRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {}

void Player::jump() {
	if (this->_grounded) {
		_y -= 5;

		_currentSurface = NOTHING;
		this->_dy = 0;
		this->_dy -= player_constants::JUMP;
		this->_grounded = false;

	}
}

void Player::bounce() {
	this->_dy = 0;
	this->_dy -= player_constants::BOUNCE;
	this->_grounded = false;
}

void Player::shoot(Graphics& graphics) {
	if (_weapon != NULL) {
		_weapon->shoot(graphics);
	}
}

void Player::moveLeft() {
	if (_lookingDown && _grounded) {
		return;
	}
	this->_dx = -player_constants::WALK_SPEED;
	if (_lookingUp == false) {
		this->playAnimation("RunLeft");
	}
	this->_facing = LEFT;
}

void Player::moveRight() {
	
	if (_lookingDown && _grounded) {
		return;
	}
	this->_dx = player_constants::WALK_SPEED;
	if (_lookingUp == false) {
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}

void Player::lookForward() {
	_lookingDown = false;
	_lookingUp = false;
}

void Player::lookUp(){
	_lookingUp = true;
	if (_facing == RIGHT) {
		if (_dx != 0) {
			this->playAnimation("LookUpRunRight");
		}
		else
			this->playAnimation("LookUpIdleRight");
	}
	else {
		if (_dx != 0) {
			this->playAnimation("LookUpRunLeft");
		}
		else
			this->playAnimation("LookUpIdleLeft");
	}
}

void Player::lookDown() {
	_lookingDown = true;
	if (_grounded) {
		if (_facing == RIGHT) 
				this->playAnimation("LookBackwardsRight");
		else
				this->playAnimation("LookBackwardsLeft");
	}
	else {
		if (_facing == RIGHT)
			this->playAnimation("LookDownIdleRight");
		else
			this->playAnimation("LookDownIdleLeft");
	}
	
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	if (!_lookingUp && !_lookingDown) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
	}
}

//void handleTileCollisions
//Handles collisions with ALL tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	//Figure out what side the collision happened on and move the player accordingly

	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0;
				this->_y = others.at(i).getBottom() + 1;
				if (this->_grounded) {
					this->_dx = 0;
					this->_x -= this->_facing == RIGHT ? 1.0f : -1.0f;
				}
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true;
				_currentSurface = RECTANGLE;
				_lastCollidedFloorRect = others.at(i);
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				_lastCollidedLeftRect = others.at(i);
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				_lastCollidedRightRect = others.at(i);
				break;
			}
		}
	}
}


/**Handles collisions with ALL slopes the player is colliding with (treating them as BELOW the player)**/
void Player::handleSlopeCollisions(std::vector<Slope> &others) {
	_currentSurface = SLOPE;
	
	for (int i = 0; i < others.size(); i++) {
		_lastCollidedSlope = others.at(i);

		//Calculate where on the slope the player's bottom center is touching
		//and use y=mx+b to figure out the y position to place him at
		//First calculate "b" (slope intercept) using one of the points (b = y - mx)
		int b = (others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));

		//Now get player's center x
		int centerX = this->_boundingBox.getCenterX();

		//Now pass that X into the equation y = mx + b (using our newly found b and x) to get the new y position
		int newY = (others.at(i).getSlope() * centerX) + b - 10; //8 is temporary to fix a problem
		
		this->_y = newY - this->_boundingBox.getHeight();
		this->_grounded = true;
		
	}
}

/**Handles enemy collision with player**/
void Player::handleEnemyCollisions(Level& level, std::vector<Enemy*> others) {
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = this->getCollisionSide(others.at(i)->getBoundingBox());
		if (collisionSide == sides::BOTTOM) {
			_grounded = true;
		}
		//Call the enemy to handle the player collision
		others.at(i)->handlePlayerCollision(level, *this, collisionSide, this->getBoundingBox());
	}
}

void Player::handleDoorCollision(std::vector<Door> &others, Level& level, Graphics &graphics, Game& game) {
	for (int i = 0; i < others.size(); i++) {
		if (_grounded && _lookingDown) {
			level = Level(others.at(i).getDestination(), graphics);
			_x = level.getPlayerSpawnPoint().x;
			_y = level.getPlayerSpawnPoint().y;
		}
	}
}

void Player::handleWeaponCollisions(Level& level, std::vector<Weapon*> others) {
	for (int i = 0; i < others.size(); i++) {
		if (_grounded && _lookingDown) {
			//PICK UP GUN
			_weapon = others.at(i);
			_weapon->_isHeld = true;
		}
	}
}

//Modify health. If health is lost, become invulnerable for 2 seconds
void Player::gainHealth(int x) {
	if (x > 0) {
		_currentHealth = _currentHealth + x;
	}
	if (x < 0) {
		if (_invulnerable)
			return;
		else {
			_currentHealth = _currentHealth + x;
			_invulnerable = true;
		}
	}
}

void Player::updateCollisions(Level& level, Game& game, Graphics& graphics) {
	/****Check collisions of player with level****/
	std::vector<Rectangle> collidingRects = level.checkTileCollisions(getBoundingBox());
	std::vector<Slope> collidingSlopes = level.checkSlopeCollisions(getBoundingBox());
	std::vector<Door> collidingDoors = level.checkDoorCollisions(getBoundingBox());
	std::vector<Enemy*> collidingEnemies = level.checkEnemyCollisions(getBoundingBox());
	std::vector<Weapon*> collidingWeapons = level.checkWeaponCollisions(getBoundingBox());
	//Player collided with at least one tile. Handle it.
	if (collidingRects.size() > 0) {
		handleTileCollisions(collidingRects);
	}
	if (collidingSlopes.size() > 0) {
		//Player collided with at least one slope. Handle it.
		handleSlopeCollisions(collidingSlopes);
	}
	if (collidingDoors.size() > 0) {
		//Player collided with a door
		handleDoorCollision(collidingDoors, level, graphics, game);
	}
	if (collidingEnemies.size() > 0) {
		//Player collided with at least one enemy
		handleEnemyCollisions(level, collidingEnemies);
	}
	if (collidingWeapons.size() > 0) {
		handleWeaponCollisions(level, collidingWeapons);
	}
	
}

void Player::updateWeapon(float elapsedTime, Graphics& graphics) {
	if (_facing == LEFT) {
		if (_lookingUp)
			_weapon->playAnimation("UpLeft");
		else
			_weapon->playAnimation("Left");
	}
	if (_facing == RIGHT) {
		if (_lookingUp)
			_weapon->playAnimation("UpRight");
		else
			_weapon->playAnimation("Right");
	}


	_weapon->setX(getX() + getBoundingBox().getWidth() / 2 - 20);
	_weapon->setY(getY() + 3);

}

void Player::update(float elapsedTime, Level& level, Game& game, Graphics& graphics) {
	
	//UPDATE QUOTES COLLISIONS
	updateCollisions(level, game, graphics);
	
	//UPDATE WEAPON
	if (_weapon != NULL) {
		updateWeapon(elapsedTime, graphics);
	
	}

	//UPDATE INVULNERABILITY
	if (_invulnerable) {
		if (damageTimer >= player_constants::DAMAGE_TIMER) {
			_invulnerable = false;
			damageTimer = 0;
			setVisible(true);
		}
		else if (damageTimer < player_constants::DAMAGE_TIMER) {
			damageTimer += elapsedTime;
			blipTimer += elapsedTime;
		}
		if (blipTimer >= player_constants::DAMAGE_BLIP) {
			if (_visible) {
				_visible = false;
			}
			else {
				_visible = true;
			}
			blipTimer = 0;
		}
	}
	//UPDATE _GROUNDED: if he has walked beyond the dimensions of his current surface
	if(_currentSurface == RECTANGLE){
		if (getBoundingBox().getRight() < _lastCollidedFloorRect.getLeft() || getBoundingBox().getLeft() > _lastCollidedFloorRect.getRight()) {
			_grounded = false;
			_currentSurface = NOTHING;
		}
	}
	else if (_currentSurface == SLOPE) {
		if (getBoundingBox().getLeft() < _lastCollidedSlope.getP1().x && getBoundingBox().getRight() < _lastCollidedSlope.getP1().x
			&& getBoundingBox().getLeft() < _lastCollidedSlope.getP2().x && getBoundingBox().getRight() < _lastCollidedSlope.getP2().x) {
			_grounded = false;
		}
		if (getBoundingBox().getLeft() > _lastCollidedSlope.getP1().x && getBoundingBox().getRight() > _lastCollidedSlope.getP1().x
			&& getBoundingBox().getLeft() > _lastCollidedSlope.getP2().x && getBoundingBox().getRight() > _lastCollidedSlope.getP2().x) {
			_grounded = false;
		}
	}


	//APPLY GRAVITY : Only if Quote is on a slope (in which case we need to constantly update his y), or if he is not on the ground
	if (!_grounded || _currentSurface == SLOPE) {
		if (this->_dy <= player_constants::GRAVITY_CAP) {
			this->_dy += player_constants::GRAVITY * elapsedTime;
		}
	}

	//Move by dx
	this->_x += this->_dx * elapsedTime;
	//Move by dy
	this->_y += this->_dy * elapsedTime;

	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
	if(_weapon != NULL)
		_weapon->draw(graphics);
}
