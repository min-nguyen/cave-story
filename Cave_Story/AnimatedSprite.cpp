/*AnimatedSprite class
Holds data and logic for animated sprites*/
#include "animatedsprite.h"
#include "graphics.h"

AnimatedSprite::AnimatedSprite(){}

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filePath, int srcX, int srcY, int width, int height,
	float _x, float _y, float timeToUpdate) :
		//Create fields for baseclass 'Sprite'
		Sprite(graphics, filePath, srcX, srcY, width, height, _x, _y),
		_frameIndex(0),
		_timeElapsed(0),
		_timeToUpdate(timeToUpdate),
		_visible(true),
		_currentAnimationOnce(false),
		_currentAnimation("")
{}

void AnimatedSprite::addAnimationV2(std::vector<SDL_Rect> animationRects, std::string name, Vector2 offset) {
	this->_animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, animationRects));
	this->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
	std::vector<SDL_Rect> animationRectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect rectangle = { (i + x)*width, y, width, height };
		animationRectangles.push_back(rectangle);
	}
	this->_animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, animationRectangles));
	this->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::resetAnimations() {
	this->_animations.clear();
	this->_offsets.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
	this->_currentAnimationOnce = once;
	if (this->_currentAnimation != animation) {
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
	}
}

void AnimatedSprite::setVisible(bool visible) {
	this->_visible = visible;
}

void AnimatedSprite::stopAnimation() {
	this->_frameIndex = 0;
	this->animationDone(this->_currentAnimation);
}



void AnimatedSprite::update(float elapsedTime) {
	Sprite::update();

	if (this->_timeElapsed > this->_timeToUpdate) {
		if (this->_frameIndex >= this->_animations.at(_currentAnimation).size() - 1) {
			if (_currentAnimationOnce)
				this->setVisible(false);
			this->stopAnimation();
		}
		else {
			_frameIndex++;
		}
		_timeElapsed = 0;
	}
	else {
		this->_timeElapsed += elapsedTime;
	}
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y){
	if (this->_visible) {
		SDL_Rect destRect;
		destRect.x = (x ) + this->_offsets.at(_currentAnimation).x;
		destRect.y = (y ) + this->_offsets.at(_currentAnimation).y;
		destRect.w = this->_sourceRect.w * globals::SPRITE_SCALE;
		destRect.h = this->_sourceRect.h * globals::SPRITE_SCALE;
		//srcRect: From map of animations, retrieve list of rectangles corresponding to the current animation name.
		//Then retrieve the rectangle corresponding to the current frame index
		SDL_Rect srcRect = this->_animations.at(this->_currentAnimation).at(_frameIndex);
		graphics.blitSurface(this->_spriteSheet, &srcRect, &destRect);
	}
}
