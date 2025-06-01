#pragma once
#include "StaticSprite.h"
#include "Animation.h"

class AnimatedSprite : public StaticSprite
{
private:
	std::map<std::string, Animation> animations;
	float currentTime;

public:
	AnimatedSprite(std::string textureName, sf::IntRect currentRect, sf::Vector2f position);
	~AnimatedSprite() = default;
	void addAnimation(std::string name, int frameCount, sf::IntRect frameSize, float frameTime, bool looping = true);
	void playAnimation(std::string name, float deltaTime);
	//frameCount, frameSize, frameTime, looping
	void render(sf::RenderTarget& target);


	void update(float deltaTime);
	void draw(float x, float y);
	void setLooping(bool looping);
	void setFrame(int frameIndex);
	int getCurrentFrame();
};

