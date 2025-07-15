#include "stdafx.h"
#include "StaticSprite.h"


StaticSprite::StaticSprite(sf::Texture& texture, sf::IntRect currentRect)
	:sprite(texture)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(currentRect);
}

StaticSprite::~StaticSprite()
{
}

/*void StaticSprite::initSprite(sf::IntRect size, sf::Texture* texture)
{
	sprite.emplace(*(texture));
	sprite->setTextureRect(size);
}*/

void StaticSprite::setPosition(sf::Vector2f position)
{
	sprite.setPosition(position);
}

void StaticSprite::setTextureRect(sf::IntRect rect)
{
	sprite.setTextureRect(rect);
}

void StaticSprite::setScale(sf::Vector2f scale)
{
	sprite.setScale(scale);
}

sf::FloatRect StaticSprite::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

sf::Vector2f StaticSprite::getPosition() const
{
	return sprite.getPosition();
}


void StaticSprite::render(sf::RenderTarget& target) const
{
	target.draw(sprite);
}
