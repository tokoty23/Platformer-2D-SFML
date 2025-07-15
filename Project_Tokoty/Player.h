#pragma once
#include "Entity.h"
#include "UserInput.h"



class Player : public Entity
{
private:
	//User Input
	std::unique_ptr<UserInput> userInput;
	
	//Initialization functions
	void initUserInput();
	void initAnmation();
	void initPhysics() override;
	void initCombatStats() override;

public:
	Player(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite);
	virtual ~Player() override = default;

	void updateMovement(sf::Time deltaTime);
	void update(sf::Time deltaTime) override;
	void render(sf::RenderTarget& target) override;
	void attack() override;
	void takeDamage(float damage) override;
	void updateAnimation(sf::Time deltaTime) override;
	void updateTimers(sf::Time deltaTime) override;
	//void setIsGrounded(bool isGrounded) override;
};