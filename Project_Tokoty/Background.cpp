#include "stdafx.h"
#include "Background.h"


Background::Background(sf::Texture& bgTexture, const sf::RenderTarget& window, sf::Vector2f parallaxFactor)
	: backgroundSprite(bgTexture), parallaxFactor(parallaxFactor), windowSize(window.getSize())
{
	sf::Vector2f textureSize = (sf::Vector2f)bgTexture.getSize();
	backgroundSprite.setOrigin(textureSize / 2.0f);
	resizeToWindow(window);
}

void Background::render(sf::RenderTarget& window)
{
	window.draw(backgroundSprite);
}

void Background::resizeToWindow(const sf::RenderTarget& window)
{
	windowSize = (sf::Vector2f)window.getSize();
	float scale = std::max(
		windowSize.x / (float)(backgroundSprite.getTexture().getSize().x),
		windowSize.y / (float)(backgroundSprite.getTexture().getSize().y)
	);
	setScale(scale, scale);
	
	
}

void Background::setParallaxFactor(float x, float y)
{
	parallaxFactor.x = x;
	parallaxFactor.y = y;
}

void Background::updateBackground(const sf::Vector2f& playerPos)
{
	backgroundSprite.setPosition(sf::Vector2f (playerPos.x * parallaxFactor.x,
		playerPos.y * parallaxFactor.y) );
}

void Background::setScale(float x, float y)
{
	backgroundSprite.setScale(sf::Vector2f(x, y));
}