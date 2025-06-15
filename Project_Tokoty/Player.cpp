#include "stdafx.h"
#include "Player.h"


void Player::initUserInput()
{
	userInput = std::make_unique<UserInput>();
}

void Player::initAnmation()
{
	if (animatedSprite)
	{
		animatedSprite->addAnimation("IDLE", 5, sf::IntRect({ 0, 0 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_LEFT", 16, sf::IntRect({ 0, 144 }, { 48, 48 }), 0.05f);
		animatedSprite->addAnimation("MOVE_RIGHT", 16, sf::IntRect({ 0, 96 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_UP", 7, sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_DOWN", 7, sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
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
	initUserInput();
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

	Entity::update(deltaTime);
}