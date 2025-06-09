#pragma once
#include "Entity.h"
#include "UserInput.h"


class Player : public Entity
{
private:
	//User Input
	std::unique_ptr<UserInput> userInput;

	//Initialization functions
	void initVariables();
	void initSprite();
	void initAnmation();
	void initPhysics() override;

public:
	Player(std::unique_ptr<Collider> collider, std::unique_ptr<AnimatedSprite> sprite);
	virtual ~Player() override = default;

	void updateMovement(float deltaTime);
	void update(float deltaTime) override;
	//void render(sf::RenderTarget& target); //Entity.h
};