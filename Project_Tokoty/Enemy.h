#pragma once
#include "Entity.h"

enum class EnemyState
{
	IDLE,
	MOVING_LEFT,
	MOVING_RIGHT,
	ATTACKING,
	DEAD
};

class Enemy : public Entity
{
protected:

	//Enemy state
	EnemyState state;
	sf::Vector2f startPosition;
	float movingRange;


	//Initialization functions
	void initAnmation();
	void initPhysics() override;
	void virtual initBehaviour();
	virtual void updateEnemyBehaviour(sf::Time deltaTime);
public:
	Enemy(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite);
	virtual ~Enemy() override = default;

	void update(sf::Time deltaTime) override;
	

};



