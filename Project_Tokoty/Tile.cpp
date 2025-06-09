#include "stdafx.h"
#include "Tile.h"

Tile::Tile(std::string textureName, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position, bool damaging, bool moveable)
	: damaging(damaging), moveable(moveable)
{

	initSprite(textureName, sizeHitbox, sizeSprite, position);
}

Tile::Tile(sf::Texture* texture, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position, bool damaging, bool moveable)
	: damaging(damaging), moveable(moveable)
{

	initSprite(texture, sizeHitbox, sizeSprite, position);
}


void Tile::initSprite(std::string textureName, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position)
{
	sprite = std::make_unique<StaticSprite>(textureName, sizeSprite, position);
	hitboxTile = std::make_unique<Collider>
		( sf::FloatRect( { position }, { float(sizeHitbox.size.x), float(sizeHitbox.size.y) } ), position, 0.0f); //PROBLEMA are treaba cumva ca am schibmat din sprite->getGlobal
	// in constructorul lui collider parametrul dat la hitbox este salvat deci chiar daca setezi positia, isi va da update la valoarea din size	
}

void Tile::initSprite(sf::Texture* texture, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position)
{
	sprite = std::make_unique<StaticSprite>(texture, sizeSprite, position);
	hitboxTile = std::make_unique<Collider>
		(sf::FloatRect({ position }, { float(sizeHitbox.size.x), float(sizeHitbox.size.y) }), position, 0.0f);
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
		if(moveable) sprite->setPosition(hitboxTile->getPosition());
		sprite->StaticSprite::render(target);
	}
	hitboxTile->renderCollider(target);
}
