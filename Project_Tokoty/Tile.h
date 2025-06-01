#pragma once
#include "Collider.h" 
#include "StaticSprite.h"
#include "AnimatedSprite.h"

class Tile
{
private:
	//Sprite
	sf::Texture texture;
	//std::optional<sf::Sprite> sprite;
	//std::unique_ptr<StaticSprite> sprite;
	std::unique_ptr<AnimatedSprite> sprite;
	//sf::Sprite sprite;
	const bool damaging;
	std::unique_ptr<Collider> hitboxTile;
public:
	//Initialization functions
	Tile(std::string textureName, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position, bool damaging = false);
	void initTexture();
	void initSprite(std::string textureName, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position);
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

