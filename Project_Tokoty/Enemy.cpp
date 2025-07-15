#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite)
	: Entity(std::move(hitbox), std::move(sprite))
{
	Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	if (!collider)
	{
		std::cerr << "ERROR Class Player: constructor" << std::endl;
		return;
	}

	sf::Vector2f hitboxSize = collider->getSize();
	sf::Vector2f hitboxPosition = collider->getPosition();
	sf::FloatRect hitboxRect = collider->getGlobalBounds();
	auto hurtbox = std::make_unique<Collider>(hitboxRect, 0.0f, ColliderType::C_HURTBOX);
	addCollider(std::move(hurtbox), ColliderKeys::E_HURTBOX);
	//hitboxRect.position.x += hitboxRect.size.x;
	auto attackbox = std::make_unique<Collider>(hitboxRect, 0.f, ColliderType::C_ATTACKBOX);
	//attackbox->setActive(true);
	std::cout << "Attackbox enemy active at spawn: " << attackbox->isActive() << std::endl;
	addCollider(std::move(attackbox), ColliderKeys::E_ATTACKBOX_DEFAULT);
	

	initPhysics();
	initBehaviour();
	initAnmation();
}

void Enemy::initAnmation()
{
	if (animatedSprite)
	{
		animatedSprite->addAnimation("IDLE", 5, sf::IntRect({ 0, 0 }, { 48, 48 }), 0.3f);
		animatedSprite->addAnimation("MOVING", 16, sf::IntRect({ 0, 96 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_UP", 7, sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("MOVE_DOWN", 7, sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("ATTACK", 7, sf::IntRect({ 0, 48 }, { 48, 48 }), 0.1f);
		animatedSprite->addAnimation("DYING", 16, sf::IntRect({ 0, 144 }, { 48, 48 }), 0.02f);
	}
}
void Enemy::initPhysics()
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

void Enemy::initBehaviour()
{
	state = EntityState::E_MOVING;
	facingRight = false;
	startPosition = Entity::getPosition();
	movingRange = 500.0f;

}

void Enemy::update(sf::Time deltaTime)
{
	//updateMovement doar modifica viteza nu si pozitia, updatePhysics modifica pozitia in functie de viteza
	updateEnemyBehaviour(deltaTime);

	Entity::update(deltaTime);
	
}

void Enemy::updateAnimation(sf::Time deltaTime)
{
	Entity::updateAnimation(deltaTime);
}

void Enemy::updateEnemyBehaviour(sf::Time deltaTime)
{
	if (isDead) return;
	attack();
	switch (state)
	{
		case EntityState::E_IDLE:
		case EntityState::E_MOVING:

			if (facingRight == true)
			{
				if (getPosition().x - startPosition.x >= movingRange)
				{
					state = EntityState::E_MOVING;
					facingRight = false;
					std::cout << "Enemy is moving right" << std::endl;
					break;
				}
				move(speed, 0.f, deltaTime);
			}
			else if (facingRight == false)
			{
				if (getPosition().x - startPosition.x <= -movingRange)
				{
					state = EntityState::E_MOVING;
					facingRight = true;
					std::cout << "Enemy is moving left" << std::endl;
					break;
				}
				move(-speed, 0.f, deltaTime);
			}
			
		break;
		case EntityState::E_ATTACKING:

			
			break;
		default:
			state = EntityState::E_IDLE;
			break;
				
	}
}