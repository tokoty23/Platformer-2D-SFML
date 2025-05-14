#include "stdafx.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent(sf::Sprite* sprite, float velocity)
	: sprite(sprite), speed(speed) {}

MovementComponent::~MovementComponent() {}

void MovementComponent::updateMovement()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) //UP
	{
		sprite->move({ 0.0f, -speed });
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) //LEFT
	{
		sprite->move({ -speed, 0.0f });
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) //DOWN
	{
		sprite->move({ 0.0f, speed });
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) //RIGHT
	{
		sprite->move({ speed, 0.0f });
	}
}