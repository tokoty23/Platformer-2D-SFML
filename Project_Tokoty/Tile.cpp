#include "stdafx.h"
#include "Tile.h"

Tile::Tile(std::string textureName, sf::IntRect size, sf::Vector2f position, bool damaging)
	: damaging(damaging)
{
	initTexture();
	initSprite(textureName, size, position);
}

void Tile::initTexture()
{
	if (!texture.loadFromFile("Textures/Soldier.png"))
	{
		std::cout << "ERROR Class Player: initTexture()";
	}
}

void Tile::initSprite(std::string textureName, sf::IntRect size, sf::Vector2f position)
{
	sprite = std::make_unique<StaticSprite>(textureName, size, position);
	hitboxTile = std::make_unique<Collider>
		( sf::FloatRect( { position }, { float(size.size.x), float(size.size.y) } ), position, 0.0f); //PROBLEMA are treaba cumva ca am schibmat din sprite->getGlobal
	// in constructorul lui collider parametrul dat la hitbox este salvat deci chiar daca setezi positia, isi va da update la valoarea din size


	//sprite.emplace(texture);
	//sprite->setTextureRect(sf::IntRect({ 0, 0 }, { 200, 200 })); //{top left corner} {wide tall}
	//sprite->setPosition({ 50.0f, 50.0f });
	//sprite->setScale({ 5.0f, 5.0f });
	
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
		sprite->render(target);
	}
	hitboxTile->renderCollider(target);
}
