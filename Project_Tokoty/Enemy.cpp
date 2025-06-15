#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite)
	: Entity(std::move(hitbox), std::move(sprite))
{
	initPhysics();
	initBehaviour();
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
	state = EnemyState::MOVING_RIGHT;
	startPosition = Entity::getPosition();
	movingRange = 500.0f;

}

void Enemy::update(float deltaTime)
{
	//updateMovement doar modifica viteza nu si pozitia, updatePhysics modifica pozitia in functie de viteza
	updateEnemyBehaviour(deltaTime);

	Entity::update(deltaTime);
	
}

void Enemy::updateEnemyBehaviour(float deltaTime)
{
	switch (state)
	{
		case EnemyState::MOVING_RIGHT:

			if (getPosition().x - startPosition.x >= movingRange)
			{
				state = EnemyState::MOVING_LEFT;
				std::cout << "Enemy is moving right" << std::endl;
				break;
			}
			move(speed, 0.f, deltaTime);
		break;
		case EnemyState::MOVING_LEFT:

			if (getPosition().x - startPosition.x <= -movingRange)
			{
				state = EnemyState::MOVING_RIGHT;
				std::cout << "Enemy is moving left" << std::endl;
				break;
			}
			move(-speed, 0.f, deltaTime);
			break;
		default:
			state = EnemyState::IDLE;
			break;
				
	}

	
}