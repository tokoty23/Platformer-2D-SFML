#pragma once
#include "Player.h"

class UIElement
{
public:
	virtual ~UIElement() = default;
	virtual void update(sf::Time deltaTime) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
	
};

