#pragma once
#include "UIElement.h"

class FPSCounter : public UIElement
{
private:
	sf::RenderWindow& window;
	sf::Font& font;
	sf::Text text;

	sf::Time updateTime; // la cat timp se actualizeaza fps
	sf::Time currentTime;
	int frameCount;
	void setPositionOnScreen();
public:
	FPSCounter(sf::RenderWindow& window, sf::Font& font);

	void update(sf::Time deltaTime) override;
	void render(sf::RenderTarget& target) override;
};

