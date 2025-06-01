#pragma once

class StaticSprite
{
protected:
	std::optional<sf::Sprite> sprite;
	sf::Texture texture;
	std::string textureName;
	sf::IntRect currentRect;
public:
	StaticSprite(std::string textureName, sf::IntRect currentRect, sf::Vector2f position);
	~StaticSprite();
	void initTexture(std::string textureName);
	void initSprite(sf::IntRect size);
	void setPosition(sf::Vector2f position);
	sf::FloatRect getGlobalBounds() const;
	void setScale(sf::Vector2f scale);
	void render(sf::RenderTarget& target);
};

