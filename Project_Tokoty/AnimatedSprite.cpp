#include "stdafx.h"
#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(std::string textureName, sf::IntRect currentRect, sf::Vector2f position)
	: StaticSprite(textureName, currentRect, position)
{
	currentTime = 0.0f;
}


void AnimatedSprite::addAnimation(std::string name, int frameCount, sf::IntRect frameSize, float frameTime, bool looping)
{
	animations.emplace(name, Animation(frameCount, frameSize, frameTime, looping));
}

void AnimatedSprite::playAnimation(std::string name, float deltaTime)
{
    if (sprite)
    {
		
        animations[name].playAnimation(sprite.value(), deltaTime); // sprite ul este std::optional trebuie apelat .value()
    }
}

void AnimatedSprite::render(sf::RenderTarget& target)
{
	if (sprite.has_value())
	{
		//std::cout << currentRect.position.x << " " << currentRect.position.y << " + "<< textureName  << std::endl;
		target.draw(*sprite);
	}
}
