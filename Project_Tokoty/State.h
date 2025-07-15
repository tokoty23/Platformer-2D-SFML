#pragma once
#include "GameEngine.h"
#include "AnimatedSprite.h"

class State
{
protected:

public:
	virtual ~State() = default;
	virtual void handleInput(GameEngine& engine, std::optional<sf::Event>& event) = 0;
	virtual void update(GameEngine& engine, sf::Time deltaTime) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
	virtual void resizeWindow(sf::RenderTarget& window) = 0;
};

