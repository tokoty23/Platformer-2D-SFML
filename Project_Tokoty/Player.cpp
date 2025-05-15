#include "stdafx.h"
#include "Player.h"


void Player::initVariables()
{
	animationState = Player_AnimationStates::IDLE;
}

void Player::initTexture()
{
	if (!texture.loadFromFile("Textures/Soldier.png"))
	{
		std::cout << "ERROR Class Player: initTexture()";
	}
}

void Player::initSprite()
{
	sprite.emplace(texture);
	currentFrame = sf::IntRect({ 0, 0 }, { 100, 100 }); //{top left corner} {wide tall}
	sprite->setTextureRect(currentFrame);
	std::cout << sprite->getGlobalBounds().size.x<< sprite->getGlobalBounds().size.x<<std::endl;
	sprite->setScale({ 5.0f, 5.0f });
	std::cout << sprite->getGlobalBounds().size.x << sprite->getGlobalBounds().size.x << std::endl;
}

void Player::initAnmation()
{

}

void Player::initPhysics()
{
	velocity = sf::Vector2f(0.0f, 0.0f);
	maxVelocity = 20.0f;
	minVelocity = 1.0f;
	acceleration = 3.0f;
	friction = 0.80f;
	gravity = 3.0f;
	maxGravity = 30.0f;
}



Player::Player()
{
	initTexture();
	initSprite();	
	initAnmation();
	initPhysics();
	initVariables();
	
}

Player::~Player()
{

}

const sf::Vector2f Player::getPosition() const
{
	return sprite->getPosition();
}

const sf::FloatRect Player::getGlobalBounds() const
{
	return sprite->getGlobalBounds();
}

void Player::setPosition(const float x, const float y)
{
	sprite->setPosition({ x, y });
}

void Player::resetVelocityY()
{
	velocity.y = 0.0f;
}

void Player::move(const float x, const float y)
{
	velocity.x += x * acceleration;
	velocity.y += y * acceleration;

	if (std::abs(velocity.x) > maxVelocity) velocity.x = maxVelocity * ((velocity.x < 0) ? -1.0f : 1.0f);
	//if (std::abs(velocity.y) > maxVelocity) velocity.y = maxVelocity * ((velocity.y < 0) ? -1.0f : 1.0f);
	
}

void Player::updatePhysics()
{
	//Gravity
	velocity.y += 1.0 * gravity;
	if (std::abs(velocity.y) > maxGravity) velocity.y = maxGravity * ((velocity.y < 0) ? -1.0f : 1.0f);

	velocity *= friction;

	if(std::abs(velocity.x) < minVelocity) velocity.x = 0.0f;
	if(std::abs(velocity.y) < minVelocity) velocity.y = 0.0f;

	sprite->move(velocity);
	std::cout << "velocity.x: " << velocity.x << "        ";
	std::cout << "velocity.y: " << velocity.y << std::endl;
}

void Player::updateMovement()
{
	animationState = Player_AnimationStates::IDLE;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) //JUMP
	{
		move(0.0f, -2.0f);
		animationState = Player_AnimationStates::JUMP;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) //LEFT
	{
		move(-2.0f, 0.0f );
		animationState = Player_AnimationStates::LEFT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) //DOWN
	{
		move(0.0f, 2.0f);
		animationState = Player_AnimationStates::FALL;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) //RIGHT
	{
		move(2.0f, 0.0f);
		animationState = Player_AnimationStates::RIGHT;
	}
}

void Player::updateAnimation()
{
	if (animationState == Player_AnimationStates::IDLE)
	{
		if (animationClock.getElapsedTime().asSeconds() >= 0.1f)
		{
			currentFrame.position.x += 100;
			currentFrame.position.y = 0;
			if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
			animationClock.restart();
			sprite->setTextureRect(currentFrame);
		}
	}
	
	else 
	{
		if (animationClock.getElapsedTime().asSeconds() >= 0.1f)
		{

			{
				switch (animationState)
				{
				case Player_AnimationStates::LEFT:
					currentFrame.position.x += 100;
					currentFrame.position.y = 200;
					if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
					sprite->setScale({ -5.0f, 5.0f });
					sprite->setOrigin(sf::Vector2f(sprite->getGlobalBounds().size.x / 5.0f, 0 ));
					break;

				case Player_AnimationStates::RIGHT:
					currentFrame.position.x += 100;
					currentFrame.position.y = 200;
					if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
					sprite->setScale({ 5.0f, 5.0f });
					sprite->setOrigin({ 0, 0 });
					break;

				case Player_AnimationStates::JUMP:
					currentFrame.position.x += 100;
					currentFrame.position.y = 300;
					if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
					break;

				case Player_AnimationStates::FALL:
					currentFrame.position.x += 100;
					currentFrame.position.y = 400;
					if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
					break;

				default:

					break;
				}
			}
			animationClock.restart();
			sprite->setTextureRect(currentFrame);
		}
	}
}

void Player::update()
{
	updateMovement();
	updateAnimation();
	updatePhysics();
}

void Player::render(sf::RenderTarget& target)
{
	if (sprite)
	{
		target.draw(*sprite);
	}
	sf::RectangleShape circle(sf::Vector2f(sprite->getGlobalBounds().size.x, sprite->getGlobalBounds().size.y));
	//circle.setRadius(50.0f);
	circle.setFillColor(sf::Color(255, 0, 0, 128)); // Red, half transparent
	circle.setPosition(sprite->getPosition());
	target.draw(circle);
	
}
