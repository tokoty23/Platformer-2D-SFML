#include "stdafx.h"
#include "Player.h"

Player::Player(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite)
	: Entity(std::move(hitbox), std::move(sprite))
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
	initCombatStats();

}

void Player::initUserInput()
{
	userInput = std::make_unique<UserInput>();
}

void Player::initAnmation()
{
	if (animatedSprite)
	{
		animatedSprite->addAnimation("IDLE",	   5,  sf::IntRect({ 0, 0 }, { 48, 48 }), 0.3f);
		animatedSprite->addAnimation("MOVING", 16, sf::IntRect({ 0, 96 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_UP",	   7,  sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_DOWN",  7,  sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("ATTACK",     7,  sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("DYING", 11, sf::IntRect({ 0, 144 }, { 48, 48 }), 0.02f);
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

void Player::initCombatStats()
{
	Entity::initCombatStats();
	attackDamage = 100.0f;
}



void Player::attack()
{
	if (isDead) return;
	if (attackCooldownTimer.isActive()) return;
	
	attackCooldownTimer.start();
	attackDurationTimer.start();
	state = EntityState::E_ATTACKING;

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

void Player::takeDamage(float damage)
{
	if (isDead) return;
	Entity::takeDamage(damage);
	invincibilityTimer.start();
	Collider* hurtbox = getCollider(ColliderKeys::E_HURTBOX);
	hurtbox->setActive(false);
}

// functia modifica doar viteza in functie de input, nu si pozitia
void Player::updateMovement(sf::Time deltaTime)
{
	if (isDead) return;

	std::vector<UserInputData>userInputData = userInput->getUserInput();
	
	if (userInputData.empty())
	{
		if (!attackDurationTimer.isActive()) state = EntityState::E_IDLE;
		return;
	}
	bool movementInputReceived = false;
		
	for (const auto& input : userInputData)
	{
		switch (input)
		{
		case UserInputData::U_MOVE_LEFT:
			move(-speed, 0.0f, deltaTime);
			movementInputReceived = true;
			if (!attackDurationTimer.isActive())
			{
				facingRight = false;
				state = EntityState::E_MOVING;
			}

			break;

		case UserInputData::U_MOVE_RIGHT:
			move(speed, 0.0f, deltaTime);
			movementInputReceived = true;
			if (!attackDurationTimer.isActive())
			{
				facingRight = true;
				state = EntityState::E_MOVING;
			}

			break;

		case UserInputData::U_MOVE_UP:
			velocity.y = -500.0f;
			if(isGrounded)
			{
				velocity.y = -800;
				isGrounded = false;
			}

			break;

		case UserInputData::U_MOVE_DOWN:
			move(0.0f, speed, deltaTime);
			animatedSprite->playAnimation("MOVE_DOWN", deltaTime);

			break;

		case UserInputData::U_ATTACK_DEFAULT:
			attack();
			//if (attackDurationTimer.isActive()) state = EntityState::E_ATTACKING;

			break;

		default:
			
			break;
		}
	}
	if (state != EntityState::E_ATTACKING)
	{
		if (movementInputReceived)
		{
			state = EntityState::E_MOVING;
		}
		else
		{
			state = EntityState::E_IDLE;
		}
	}
}

void Player::updateAnimation(sf::Time deltaTime)
{
	Entity::updateAnimation(deltaTime);
}

void Player::updateTimers(sf::Time deltaTime)
{
	Entity::updateTimers(deltaTime);
	if (isDead) return;
	if (!invincibilityTimer.isActive())
	{
		Collider* hurtBox = getCollider(ColliderKeys::E_HURTBOX);
		if (hurtBox)
		{
			hurtBox->setActive(true);
		}
	}
	else
	{
		Collider* hurtBox = getCollider(ColliderKeys::E_HURTBOX);
		if (hurtBox)
		{
			hurtBox->setActive(false);
		}
	}
}

void Player::update(sf::Time deltaTime)
{
	if (state == EntityState::E_DEAD) return;
	if (state == EntityState::E_ATTACKING)
	{
		if (!attackDurationTimer.isActive())
		{
			state = EntityState::E_IDLE;
		}
	}

	//updateMovement doar modifica viteza nu si pozitia, updatePhysics modifica pozitia in functie de viteza
	updateMovement(deltaTime);

	Entity::update(deltaTime);
}

void Player::render(sf::RenderTarget& target)
{
	if (state == EntityState::E_DEAD) return;
	if (invincibilityTimer.isActive() && (int)(invincibilityTimer.getRemainingTime().asMilliseconds()) % 200 < 100)
	{
		return; // Skip rendering every other 100ms for a blinking effect
	}
	Entity::render(target);
	//std::cout << "Player velocity.y: " << velocity.y << std::endl;
	//std::cout << "Player grounded" << isGrounded << std::endl;
	//Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	//std::cout << "Entity rendered at position: " << collider->getPosition().x << ", " << collider->getPosition().y << std::endl;
}

