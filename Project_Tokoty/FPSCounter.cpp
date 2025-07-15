#include "stdafx.h"
#include "FPSCounter.h"



FPSCounter::FPSCounter(sf::RenderWindow& window, sf::Font& font)
	: window(window), font(font), updateTime(sf::seconds(1.0f)), currentTime(sf::Time::Zero), frameCount(0), text(font)
{
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::Green);
	text.setOutlineColor(sf::Color::Black);
	text.setString("FPS: 0");
	setPositionOnScreen();
	
}

void FPSCounter::update(sf::Time deltaTime)
{
	currentTime += deltaTime;
	frameCount++;
	if (currentTime >= updateTime)
	{
		float fps = (float)(frameCount) / currentTime.asSeconds();
		text.setString("FPS: " + std::to_string((int)(fps)));

		// window.getSize() returneaza Vector2u
		sf::Vector2u windowSize = window.getSize();
		sf::FloatRect textBounds = text.getLocalBounds();
		setPositionOnScreen();
		currentTime -= updateTime;
		frameCount = 0;
	}
}

void FPSCounter::render(sf::RenderTarget& target)
{
	target.draw(text);
}

void FPSCounter::setPositionOnScreen()
{
	sf::Vector2u windowSize = window.getSize();
	sf::FloatRect textBounds = text.getLocalBounds();
	float x = (windowSize.x - textBounds.size.x) - 10;
	float y = 10;
	text.setPosition({ x, y });
}

