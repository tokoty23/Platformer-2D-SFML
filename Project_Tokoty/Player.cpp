#include "stdafx.h"
#include "Player.h"


void Player::initTexture()
{
	if (!texture.loadFromFile("Textures/test.png"))
	{
		std::cout << "ERROR Class Player: initTexture()";
	}
}

void Player::initSprite()
{
	sprite.emplace(texture);
	currentFrame = sf::IntRect({ 16, 0 }, { 16, 16 });
	sprite->setTextureRect(currentFrame);
}



Player::Player()
{
	initTexture();
	initSprite();	
}

Player::~Player()
{

}

void Player::updateMovement()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) //UP
	{
		std::cout << "W WWWWWW\n";
		sprite->move({ 0.0f, -1.0f });
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) //LEFT
	{
		std::cout << "W WWWWWW\n";
		sprite->move({ -1.0f, 0.0f });
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) //DOWN
	{
		std::cout << "W WWWWWW\n";
		sprite->move({ 0.0f, 1.0f });
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) //RIGHT
	{
		std::cout << "W WWWWWW\n";
		sprite->move({ 1.0f, 0.0f });
	}
}

void Player::update()
{
	updateMovement();
}

void Player::render(sf::RenderTarget& target)
{
	if (sprite)
	{
		target.draw(*sprite);
	}
}
