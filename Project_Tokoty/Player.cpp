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
		animatedSprite->addAnimation("IDLE",	   5,  sf::IntRect({ 0, 0 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_LEFT",  16, sf::IntRect({ 0, 144 }, { 48, 48 }), 0.05f);
		animatedSprite->addAnimation("MOVE_RIGHT", 16, sf::IntRect({ 0, 96 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_UP",	   7,  sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_DOWN",  7,  sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("ATTACK",     7,  sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
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
	: Entity(std::move(hitbox), std::move(sprite)), state(PlayerState::P_IDLE)
{

	Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	if (!collider)
	{
		std::cerr << "ERROR Class Player: constructor" << std::endl;
		return;
	}

	sf::FloatRect hitboxRect = collider->getGlobalBounds();
	auto hurtbox = std::make_unique<Collider>(hitboxRect, 0.0f, ColliderType::C_HURTBOX);
	addCollider(std::move(hurtbox), ColliderKeys::E_HURTBOX);
	hitboxRect.position.x += hitboxRect.size.x;
	auto attackbox = std::make_unique<Collider>(hitboxRect, 0.f, ColliderType::C_ATTACKBOX);
	//attackbox->setActive(true); 
	addCollider(std::move(attackbox), ColliderKeys::E_ATTACKBOX_DEFAULT);
	
	initUserInput();
	initPhysics();
	initAnmation();

}

void Player::attack()
{
	std::cout << attackCooldownTimer.isActive() << std::endl;
	if (attackCooldownTimer.isActive()) return;
	

	attackCooldownTimer.start();
	attackDurationTimer.start();
	state = PlayerState::P_ATTACKING;

	Collider* attackbox = getCollider(ColliderKeys::E_ATTACKBOX_DEFAULT);
	Collider* hitbox = getCollider(ColliderKeys::E_HITBOX);
	if (attackbox && hitbox)
	{
		sf::Vector2f offset = getColliderOffset(ColliderKeys::E_ATTACKBOX_DEFAULT_INITIAL);
		if (facingRight)
		{

		}
		else
		{
			offset.x = -offset.x; 
		}
		collidersOffset[ColliderKeys::E_ATTACKBOX_DEFAULT] = offset;
	}
	attackbox->setActive(true);
}

// functia modifica doar viteza in functie de input, nu si pozitia
void Player::updateMovement(sf::Time deltaTime)
{

	std::vector<UserInputData>userInputData = userInput->getUserInput();
	
	if (userInputData.empty())
	{
		if (!attackDurationTimer.isActive()) state = PlayerState::P_IDLE;
		return;
	}
	bool movementInputReceived = false;
		
	for (const auto& input : userInputData)
	{
		switch (input)
		{
		case UserInputData::U_MOVE_LEFT:
			move(-speed, 0.0f, deltaTime);
			facingRight = false; 
			movementInputReceived = true;
			if (!attackDurationTimer.isActive()) state = PlayerState::P_MOVING;

			break;

		case UserInputData::U_MOVE_RIGHT:
			move(speed, 0.0f, deltaTime);
			facingRight = true;
			movementInputReceived = true;
			if (!attackDurationTimer.isActive()) state = PlayerState::P_MOVING;

			break;

		case UserInputData::U_MOVE_UP:
			velocity.y = -600.0f;

			break;

		case UserInputData::U_MOVE_DOWN:
			move(0.0f, speed, deltaTime);
			animatedSprite->playAnimation("MOVE_DOWN", deltaTime);

			break;

		case UserInputData::U_ATTACK_DEFAULT:
			attack();
			//if (attackDurationTimer.isActive()) state = PlayerState::P_ATTACKING;

			break;

		default:
			
			break;
		}
	}
	if (state != PlayerState::P_ATTACKING)
	{
		if (movementInputReceived)
		{
			state = PlayerState::P_MOVING;
		}
		else
		{
			state = PlayerState::P_IDLE;
		}
	}
}

void Player::updateAnimation(sf::Time deltaTime)
{
	switch (state)
	{
		case PlayerState::P_IDLE:
			animatedSprite->playAnimation("IDLE", deltaTime);
			break;
		case PlayerState::P_MOVING:
			if (facingRight)
				animatedSprite->playAnimation("MOVE_RIGHT", deltaTime);
			else
				animatedSprite->playAnimation("MOVE_LEFT", deltaTime);
			break;
		case PlayerState::P_ATTACKING:
			if (facingRight)
				animatedSprite->playAnimation("ATTACK", deltaTime);
			else
				animatedSprite->playAnimation("ATTACK", deltaTime, true);
			break;
			
			break;
		default:
			animatedSprite->playAnimation("IDLE", deltaTime);
			break;
	}
}

void Player::update(sf::Time deltaTime)
{
	if (state == PlayerState::P_ATTACKING)
	{
		if (!attackDurationTimer.isActive())
		{
			state = PlayerState::P_IDLE;
		}
	}

	//updateMovement doar modifica viteza nu si pozitia, updatePhysics modifica pozitia in functie de viteza
	updateMovement(deltaTime);

	Entity::update(deltaTime);
}