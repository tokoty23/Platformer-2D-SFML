#pragma once
#include "StaticSprite.h"
#include "Animation.h"

class AnimatedSprite : public StaticSprite
{
private:
	std::map<std::string, Animation> animations;
	std::string currentAnimationName;

public:
	AnimatedSprite(sf::Texture& texture, sf::IntRect currentRect);
	~AnimatedSprite() = default;
	void addAnimation(std::string name, int frameCount, sf::IntRect frameSize, float frameTime, bool looping = true);
	void playAnimation(std::string name, sf::Time deltaTime, bool mirrored = false);
	void changeAnimation(std::string name);
	//frameCount, frameSize, frameTime, looping
	void update(sf::Time deltaTime);
	void render(sf::RenderTarget& target) const;
	void render(sf::RenderTarget& target);


	//void update(sf::Time deltaTime);
};

