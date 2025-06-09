#include "stdafx.h"
#include "StaticSprite.h"

StaticSprite::StaticSprite(std::string textureName, sf::IntRect currentRect, sf::Vector2f position)
{

	this->currentRect = currentRect;
	this->textureName = textureName;
	initTexture(textureName);
	initSprite(currentRect);
	setPosition(position);
}

StaticSprite::StaticSprite(sf::Texture* texture, sf::IntRect currentRect, sf::Vector2f position)
{
	this->currentRect = currentRect;
	initSprite(currentRect, texture);
	setPosition(position);
}

StaticSprite::~StaticSprite()
{
}

void StaticSprite::initTexture(std::string textureName)
{
	if (!texture.loadFromFile("Textures/" + textureName))
	{
		std::cerr << "ERROR Class StaticSprite: initTexture(): " << textureName << std::endl;
	}
}



void StaticSprite::initSprite(sf::IntRect size)
{
	sprite.emplace(texture);
	sprite->setTextureRect(size);
}

void StaticSprite::initSprite(sf::IntRect size, sf::Texture* texture)
{
	sprite.emplace(*(texture));
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

void StaticSprite::setScale(sf::Vector2f scale)
{
	if (sprite)
	{
		sprite->setScale(scale);
	}
}

void StaticSprite::render(sf::RenderTarget& target)
{
	if (sprite.has_value())
	{
		target.draw(*sprite);
	}
}
