#pragma once
#include "Collider.h" 
#include "StaticSprite.h"
#include "AnimatedSprite.h"



class Tile
{
private:
	//Sprite
	StaticSprite sprite;
	std::unique_ptr<Collider> hitboxTile;

	const bool damaging;
	const bool moveable;
	
public:
	Tile(sf::Texture& texture, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position, bool damaging = false, bool movable = false);

	// Rule of Five https://en.cppreference.com/w/cpp/language/rule_of_three.html
	Tile(const Tile&) = delete;
	Tile& operator=(const Tile&) = delete;
	Tile(Tile&&) noexcept = default;
	Tile& operator=(Tile&&) noexcept = default;
	virtual ~Tile() = default;

	const sf::FloatRect getGlobalBounds() const;
	Collider* getCollider() const;
	void setScale(sf::Vector2f scale);

	void update();
	void render(sf::RenderTarget& target) const;

};


