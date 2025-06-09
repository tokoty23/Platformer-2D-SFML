#include "stdafx.h"
#include "Player.h"


void Player::initVariables()
{
	userInput = std::make_unique<UserInput>();
}

void Player::initSprite()
{

}

void Player::initAnmation()
{
	if (animatedSprite)
	{
		animatedSprite->addAnimation("IDLE", 4, sf::IntRect({ 0, 0 }, { 100, 100 }), 0.2f);
		animatedSprite->addAnimation("MOVE_LEFT", 4, sf::IntRect({ 0, 100 }, { 100, 100 }), 0.2f);
		animatedSprite->addAnimation("MOVE_RIGHT", 4, sf::IntRect({ 0, 200 }, { 100, 100 }), 0.2f);
		animatedSprite->addAnimation("MOVE_UP", 4, sf::IntRect({ 0, 300 }, { 100, 100 }), 0.2f);
		animatedSprite->addAnimation("MOVE_DOWN", 4, sf::IntRect({ 0, 400 }, { 100, 100 }), 0.2f);
	}
}

void Player::initPhysics()
{
	speed = 400.0f;
	velocity = sf::Vector2f(0.0f, 0.0f);
	maxVelocity = 300.0f;
	minVelocity = 1.0f;
	acceleration = 1000.0f;
	friction = 0.90f;
	gravity = 1200.0f;
	maxGravity = 3000.0f;
}



Player::Player(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite)
	: Entity(std::move(hitbox), std::move(sprite))
{
	initVariables();
	initPhysics();
	initAnmation();

}


// functia modifica doar viteza in functie de input, nu si pozitia
void Player::updateMovement(float deltaTime)
{

	std::vector<UserInputData>userInputData = userInput->getUserInput();

	for (const auto& input : userInputData)
	{
		switch (input)
		{
		case UserInputData::U_MOVE_LEFT:
			move(-speed, 0.0f, deltaTime);
			animatedSprite->playAnimation("MOVE_LEFT", deltaTime);
			std::cout << "Player moving left" << std::endl;

			break;

		case UserInputData::U_MOVE_RIGHT:
			move(speed, 0.0f, deltaTime);
			animatedSprite->playAnimation("MOVE_RIGHT", deltaTime);

			break;

		case UserInputData::U_MOVE_UP:
			velocity.y = -600.0f;
			//move(0.0f, -ENTITY_SPEED, deltaTime);
			animatedSprite->playAnimation("MOVE_UP", deltaTime);

			break;

		case UserInputData::U_MOVE_DOWN:
			move(0.0f, speed, deltaTime);
			animatedSprite->playAnimation("MOVE_DOWN", deltaTime);

			break;

		default:
			break;
		}
	}
}

void Player::update(float deltaTime)
{
	//updateMovement doar modifica viteza nu si pozitia, updatePhysics modifica pozitia in functie de viteza
	updateMovement(deltaTime);
	updatePhysics(deltaTime);
}