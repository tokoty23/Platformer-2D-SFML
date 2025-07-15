#include "stdafx.h"
#include "Tile.h"


Tile::Tile(sf::Texture& texture, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position, bool damaging, bool moveable)
	: sprite(texture, sizeHitbox), damaging(damaging), moveable(moveable)
{
	sprite.setTextureRect(sizeSprite);
	sprite.setPosition(position);

	hitboxTile = std::make_unique<Collider>
		(sprite.getGlobalBounds(), 0.0f); //PROBLEMA are treaba cumva ca am schibmat din sprite->getGlobal
	// in constructorul lui collider parametrul dat la hitbox este salvat deci chiar daca setezi positia, isi va da update la valoarea din size	

}


const sf::FloatRect Tile::getGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

Collider* Tile::getCollider() const
{
	return hitboxTile.get();
}

void Tile::setScale(sf::Vector2f scale)
{
	sprite.setScale(scale);
	hitboxTile->setScale(scale);
}

void Tile::update()
{
	if (moveable) sprite.setPosition(hitboxTile->getPosition());
}

void Tile::render(sf::RenderTarget& target) const
{
	
	sprite.StaticSprite::render(target);
	//hitboxTile->renderCollider(target);
}
