/*AnimatedSprite class
Holds data and logic for animated sprites - thus will contain a list of source rectangles for each
set of animations. (Whereas Sprite class only contains one src rectangle)*/

#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include "sprite.h"
#include <map>
#include <string>
#include "globals.h"
#include <vector>

/*NOTE: An animation is a collection of extracted sprites which form the animation
		The animation's name is therefore representative of a collection of sprites, e.g QuoteRunLeft*/

class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	/*AnimatedSprite
		graphics - the main graphics object we are using (window, renderer, etc)
		filePath - filename of spritesheet
		srcX, srcY - coordinates on spritesheet initially
		width, height - dimensions of rect in spritesheet
		posX, posY - coordinates of where to draw to
		timeToUpdate - time between frame changes*/
	AnimatedSprite(Graphics &graphics, const std::string &filePath, int srcX, int srcY, int width, int height,
		float posX, float posY, float timeToUpdate);
	/*playAnimation
	Sets new current animation with frame index 0, provided it is not already the current animation
		animation - name of animation to play
		once - whether animation should play once, or if it should loop*/
	void playAnimation(std::string animation, bool once = false);
	/*void update
	Updates the animated sprite (using timer).
	Increment frame index*/
	virtual void update(float elapsedTime);
	/*void draw
	Draws sprite to screen
	x, y - coordinates of the destination rectangle where we are drawing to*/
	virtual void draw(Graphics &graphics, int x, int y);
	
protected:
	/*void setupAnimations
	A required function that sets up and adds all animations for an animated sprite to map _animations*/
	virtual void setupAnimations() = 0;
	/*void animationDone
	Logic that happens when an animation ends*/
	virtual void animationDone(std::string currentAnimation) = 0;
	/*_timeToUpdate
	Amount of time between frames*/
	double _timeToUpdate;
	/*_currentAnimationOnce
	Whether or not an animation is only played once*/
	bool _currentAnimationOnce;
	/*_currentAnimation
	Name of animation currently playing*/
	std::string _currentAnimation;
	/*Given list of pre-created rects*/
	void addAnimationV2(std::vector<SDL_Rect> rectList, std::string name, Vector2 offset);
	/*void addAnimation
	Adds an animation to the map of animations for the sprite
	frames - number of frames in the animation
	name - name of animation
	x, y - coordinates of the source sprite from the spritesheet
	width, height - dimensions of rect from spritesheet*/
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);
	/*void resetAnimations
	Resets all animations associated with this sprite*/
	void resetAnimations();
	/*stopAnimation
	Stops current animation*/
	void stopAnimation();
	/*void setVisible
	Changes visibility of the animated sprite*/
	void setVisible(bool visible);
	/*visible
	whether animation is currently visible*/
	bool _visible;
private:
	/*_animations
	Map of name of the animation, and the corresponding list of rectangles which are all the animations
	that the sprite has (with respect to the sprite sheet we take the sprites from).*/
	std::map<std::string, std::vector<SDL_Rect>> _animations;
	/*_offsets
	Offsets for error/readjustment.*/
	std::map<std::string, Vector2> _offsets;
	/*frameIndex
	frame we are currently at*/
	int _frameIndex;
	/*timeElapsed
	time data for our timer*/
	double _timeElapsed;
};
#endif