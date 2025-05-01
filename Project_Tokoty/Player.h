#pragma once


class Player
{
private:
	sf::Texture texture;
	std::optional<sf::Sprite> sprite;
	
	sf::IntRect currentFrame;

	void initTexture();
	void initSprite();
	
public:
	Player();
	virtual ~Player();

	void updateMovement();
	void update();
	void render(sf::RenderTarget& target);
};

