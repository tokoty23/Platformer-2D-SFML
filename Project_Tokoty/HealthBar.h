#pragma once
#include "GameConstants.h"
#include "UIElement.h"

class HealthBar : public UIElement
{
private:
	Player* player;

	sf::Texture healthBarTexture;
	std::vector<StaticSprite> hearts;

	sf::RectangleShape healthBarBackground;
	sf::RectangleShape healthBarForeground;

	sf::Vector2f position;
	sf::IntRect heartRect;
	sf::IntRect emptyHeartRect;
	float offset;

public:
	HealthBar(Player* player);

	void update(sf::Time deltaTime) override;
	void render(sf::RenderTarget& target) override;
};

