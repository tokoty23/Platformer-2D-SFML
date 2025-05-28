#include "stdafx.h"
#include "StaticSprite.h"

StaticSprite::StaticSprite(std::string textureName, sf::IntRect size, sf::Vector2f position)
{
	this->textureName = textureName;
	initTexture(textureName);
	initSprite(size);
	setPosition(position);
}

StaticSprite::~StaticSprite()
{
}

void StaticSprite::initTexture(std::string textureName)
{
	if (!texture.loadFromFile("Textures/" + textureName))
	{
		std::cout << "ERROR Class StaticSprite: initTexture()";
	}
}

void StaticSprite::initSprite(sf::IntRect size)
{
	sprite.emplace(texture);
	sprite->setTextureRect(size);
}

void StaticSprite::setPosition(sf::Vector2f position)
{
	if (sprite.has_value())
	{
		sprite->setPosition(position);
	}
}

sf::FloatRect StaticSprite::getGlobalBounds() const
{
	if (sprite.has_value())
	{
		return sprite->getGlobalBounds();
	}
	return sf::FloatRect();
}

void StaticSprite::render(sf::RenderTarget& target)
{
	if (sprite.has_value())
	{
		target.draw(*sprite);
	}
}
