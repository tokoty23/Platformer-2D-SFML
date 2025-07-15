#pragma once
#include "UIElement.h"
#include "Player.h"
#include "HealthBar.h"
#include "FPSCounter.h"

class HUD
{
private:
	sf::RenderWindow& window;

	std::vector<std::unique_ptr<UIElement>> elements;
	sf::Font font;

public:
	HUD(sf::RenderWindow& window, Player* player, sf::Font& font);
	~HUD() = default;	
	void update(sf::Time deltaTime);
	void render(sf::RenderTarget& target);
};

