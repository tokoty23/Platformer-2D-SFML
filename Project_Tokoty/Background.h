#pragma once
class Background
{
private:
	sf::Sprite backgroundSprite;
	sf::Vector2f parallaxFactor;
	sf::Vector2f windowSize;
public:
	Background(sf::Texture& bgTexture, const sf::RenderTarget& window, sf::Vector2f parallaxFactor = sf::Vector2f(0, 0) );
	void render(sf::RenderTarget& window);
	void setParallaxFactor(float x, float y);
	void resizeToWindow(const sf::RenderTarget& window);
	void updateBackground(const sf::Vector2f& playerPos);
	void setScale(float x, float y);
};

