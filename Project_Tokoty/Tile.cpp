#include "stdafx.h"
#include "Tile.h"

Tile::Tile(bool damaging) 
	: damaging(damaging)
{
	initTexture();
	initSprite();
}

void Tile::initTexture()
{
	if (!texture.loadFromFile("Textures/Soldier.png"))
	{
		std::cout << "ERROR Class Player: initTexture()";
	}
}

void Tile::initSprite()
{
	sprite.emplace(texture);
	
	sprite->setTextureRect(sf::IntRect({ 0, 0 }, { 200, 200 })); //{top left corner} {wide tall}
	sprite->setPosition({ 50.0f, 50.0f });
	//sprite->setScale({ 5.0f, 5.0f });
	hitboxTile = std::make_unique<Collider>(sprite->getGlobalBounds(), 0.0f);
}

const sf::FloatRect Tile::getGlobalBounds() const
{
	return sprite->getGlobalBounds();
}

void Tile::update()
{// Update the hitbox position to match the sprite position
	//hitboxTile = std::make_unique<Collider>(sprite->getGlobalBounds(), 0.0f); // collider size is the same as sprite size
	//hitboxTile->move(sprite->getPosition().x, sprite->getPosition().y);

}

void Tile::render(sf::RenderTarget& target)
{
	if (sprite)
	{
		sprite->setPosition(hitboxTile->getPosition());
		target.draw(*sprite);
	}
	hitboxTile->renderCollider(target);
}
