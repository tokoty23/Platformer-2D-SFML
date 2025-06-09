#pragma once
#include "Collider.h" 
#include "StaticSprite.h"
#include "AnimatedSprite.h"



class Tile
{
private:
	//Sprite
	std::unique_ptr<StaticSprite> sprite;
	const bool damaging;
	const bool moveable;
	std::unique_ptr<Collider> hitboxTile;
public:
	//Initialization functions
	Tile(std::string textureName, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position, bool damaging = false, bool movable = false);
	Tile(sf::Texture* texture, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position, bool damaging = false, bool movable = false);
	void initSprite(std::string textureName, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position);
	void initSprite(sf::Texture* texture, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position);
	const sf::FloatRect getGlobalBounds() const;

	Collider* getCollider() const
	{
		return hitboxTile.get();
	}
	
	void setScale(sf::Vector2f scale)
	{
		sprite->setScale(scale);
		hitboxTile->setScale(scale);
	}
	void update();
	void render(sf::RenderTarget& target);

};


