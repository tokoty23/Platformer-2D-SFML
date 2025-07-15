#pragma once

class StaticSprite
{
protected:
	sf::Sprite sprite;
	//std::optional<sf::Sprite> sprite;
	//sf::Texture texture;
	//std::string textureName;
	//sf::IntRect currentRect;
public:
	//StaticSprite(std::string textureName, sf::IntRect currentRect, sf::Vector2f position);
	//StaticSprite(sf::Texture* texture, sf::IntRect currentRect, sf::Vector2f position);
	StaticSprite(sf::Texture& texture, sf::IntRect currentRect);
	~StaticSprite();
	void initTexture(std::string textureName);
	void initSprite(sf::IntRect size);
	void initSprite(sf::IntRect size, sf::Texture* texture);

	void setPosition(sf::Vector2f position);
	void setTextureRect(sf::IntRect rect);
	void setScale(sf::Vector2f scale);
	sf::FloatRect getGlobalBounds() const;
	sf::Vector2f getPosition() const;
	
	void render(sf::RenderTarget& target) const;
};

