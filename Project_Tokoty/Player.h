#pragma once
#include "Entity.h"
#include "UserInput.h"

enum PlayerState
{
	P_IDLE,
	P_MOVING,
	P_JUMPING,
	P_FALLING,
	P_ATTACKING
};

class Player : public Entity
{
private:
	//User Input
	std::unique_ptr<UserInput> userInput;
	PlayerState state;


	//Initialization functions
	void initUserInput();
	void initAnmation();
	void initPhysics() override;

public:
	Player(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite);
	virtual ~Player() override = default;

	void updateMovement(sf::Time deltaTime);
	void update(sf::Time deltaTime) override;
	void attack() override;
	void updateAnimation(sf::Time deltaTime) override;
};