#include "stdafx.h"
#include "HUD.h"

HUD::HUD(sf::RenderWindow& window, Player* player, sf::Font& font)
	: window(window), font(font)
{
	elements.push_back(std::make_unique<HealthBar>(player));
	elements.push_back(std::make_unique<FPSCounter>(window, font));
}

void HUD::update(sf::Time deltaTime)
{
	for (auto& element : elements)
	{
		element->update(deltaTime);
	}
}

void HUD::render(sf::RenderTarget& target)
{
	for (auto& element : elements)
	{
		element->render(target);
	}
}

