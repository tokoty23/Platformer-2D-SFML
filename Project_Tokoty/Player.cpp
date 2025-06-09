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

}

void Player::initPhysics()
{
	ENTITY_SPEED = 400.0f;
	velocity = sf::Vector2f(0.0f, 0.0f);
	maxVelocity = 300.0f;
	minVelocity = 1.0f;
	acceleration = 1000.0f;
	friction = 0.90f;
	gravity = 1200.0f;
	maxGravity = 3000.0f;
}



Player::Player(std::string textureName, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position)
{
	hitboxPlayer = std::make_unique<Collider>
		(sf::FloatRect({ position }, { float(sizeHitbox.size.x), float(sizeHitbox.size.y) }), position, 0.0f);
	animatedSprite = std::make_unique<AnimatedSprite>(textureName, sizeSprite, position);
	//std::string textureName, sf::IntRect currentRect, sf::Vector2f position
	animatedSprite->addAnimation("IDLE", 4, sf::IntRect({ 0, 0 }, { 100, 100 }), 0.2f);
	animatedSprite->addAnimation("MOVE_LEFT", 4, sf::IntRect({ 0, 100 }, { 100, 100 }), 0.2f);
	animatedSprite->addAnimation("MOVE_RIGHT", 4, sf::IntRect({ 0, 200 }, { 100, 100 }), 0.2f);
	animatedSprite->addAnimation("MOVE_UP", 4, sf::IntRect({ 0, 300 }, { 100, 100 }), 0.2f);
	animatedSprite->addAnimation("MOVE_DOWN", 4, sf::IntRect({ 0, 400 }, { 100, 100 }), 0.2f);
	//std::string name, int frameCount, sf::IntRect frameSize, float frameTime, bool looping
		

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
	return hitboxPlayer->getPosition();
}

const sf::FloatRect Player::getGlobalBounds() const
{
	return hitboxPlayer->getGlobalBounds();
}

void Player::setPosition(const float x, const float y)
{
	hitboxPlayer->setPosition(x, y);
}

void Player::resetVelocityY()
{
	velocity.y = 0.0f;
}

void Player::move(const float x, const float y, float deltaTime)
{
	velocity.x += x * acceleration * deltaTime;
	velocity.y += y * acceleration * deltaTime;

	if (std::abs(velocity.x) > maxVelocity) velocity.x = maxVelocity * ((velocity.x < 0) ? -1.0f : 1.0f);
	//if (std::abs(velocity.y) > maxVelocity) velocity.y = maxVelocity * ((velocity.y < 0) ? -1.0f : 1.0f);
	
}

void Player::updatePhysics(float deltaTime)
{
	//Gravity
	velocity.y += 1.0f * gravity * deltaTime;
	if (std::abs(velocity.y) > maxGravity) velocity.y = maxGravity * ((velocity.y < 0) ? -1.0f : 1.0f);
	std::cout << "Velocity Y: " << velocity.y << std::endl;
	velocity *= friction;

	if (std::abs(velocity.x) < minVelocity) velocity.x = 0.0f;
	if (std::abs(velocity.y) < minVelocity) velocity.y = 0.0f;

	hitboxPlayer->move(velocity, deltaTime);
}

// functia modifica doar viteza in functie de input, nu si pozitia
void Player::updateMovement(float deltaTime)
{
	
	userInputData = userInput->getUserInput();

	for(const auto& input : userInputData)
	{
		switch (input)
		{
			case UserInputData::U_MOVE_LEFT:
				move(-ENTITY_SPEED, 0.0f, deltaTime);
				animatedSprite->playAnimation("MOVE_LEFT", deltaTime);
				
				break;

			case UserInputData::U_MOVE_RIGHT:
				move(ENTITY_SPEED, 0.0f, deltaTime);
				animatedSprite->playAnimation("MOVE_RIGHT", deltaTime);
				
				break;

			case UserInputData::U_MOVE_UP:
				velocity.y = -600.0f; // reset vertical velocity when jumping
				//move(0.0f, -ENTITY_SPEED, deltaTime);
				animatedSprite->playAnimation("MOVE_UP", deltaTime);
				
				break;

			case UserInputData::U_MOVE_DOWN:
				move(0.0f, ENTITY_SPEED, deltaTime);
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

void Player::render(sf::RenderTarget& target)
{
	if (animatedSprite)
	{
		animatedSprite->setPosition(hitboxPlayer->getPosition());	
		animatedSprite->render(target);
	}
	hitboxPlayer->renderCollider(target);

}
