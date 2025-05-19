#pragma once
#include "Collider.h" 

class Tile
{
private:
	//Sprite
	sf::Texture texture;
	std::optional<sf::Sprite> sprite;
	const bool damaging;
	std::unique_ptr<Collider> hitboxTile;
public:
	//Initialization functions
	Tile(bool damaging = false);
	void initTexture();
	void initSprite();
	const sf::FloatRect getGlobalBounds() const;

	Collider* getCollider() const
	{
		return hitboxTile.get();
	}
	
	void update();
	void render(sf::RenderTarget& target);

};

