#pragma once

#include <SFML/Graphics.hpp>

class MovementComponent
{
private:
	sf::Texture texture;
	sf::Sprite* sprite;
	//sf::IntRect currentFrame;
	float speed;

public:
	MovementComponent(sf::Sprite* sprite, float speed);
	virtual ~MovementComponent();

	void updateMovement();
	//void update();

};