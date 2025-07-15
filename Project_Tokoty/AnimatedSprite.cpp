#include "stdafx.h"
#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(sf::Texture& texture, sf::IntRect currentRect)
	: StaticSprite(texture, currentRect)
{
}


void AnimatedSprite::addAnimation(std::string name, int frameCount, sf::IntRect frameSize, float frameTime, bool looping)
{
	animations.emplace(name, Animation(frameCount, frameSize, frameTime, looping));
	currentAnimationName = name;
}

// mirrored = true animatia fi redata invers pe axa X
void AnimatedSprite::playAnimation(std::string name, sf::Time deltaTime, bool mirrored)
{		
        animations[name].playAnimation(sprite, deltaTime, mirrored); // sprite ul este std::optional trebuie apelat .value()
}

void AnimatedSprite::changeAnimation(std::string name)
{
	currentAnimationName = name;
	animations[name].resetAnimation();
}

void AnimatedSprite::update(sf::Time deltaTime)
{
	animations[currentAnimationName].playAnimation(sprite, deltaTime, false);
}

void AnimatedSprite::render(sf::RenderTarget& target)
{
    target.draw(sprite);
}

void AnimatedSprite::render(sf::RenderTarget& target) const
{
	target.draw(sprite);
}
