#pragma once

class StaticSprite
{
private:
	std::optional<sf::Sprite> sprite;
	sf::Texture texture;
	std::string textureName;
	sf::IntRect rect;
public:
	StaticSprite(std::string textureName, sf::IntRect size, sf::Vector2f position);
	~StaticSprite();
	void initTexture(std::string textureName);
	void initSprite(sf::IntRect size);
	void setPosition(sf::Vector2f position);
	sf::FloatRect getGlobalBounds() const;
	void render(sf::RenderTarget& target);
};

