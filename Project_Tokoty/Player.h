#pragma once
#include "Entity.h"
#include "UserInput.h"




class Player
{
private:
	
	//Sprite
	std::unique_ptr<AnimatedSprite> animatedSprite;

	//Hitbox
	std::unique_ptr<Collider> hitboxPlayer;

	//User Input
	std::unique_ptr<UserInput> userInput;
	std::vector<UserInputData> userInputData;
	
	//Animation

	//Physics
	float ENTITY_SPEED;
	sf::Vector2f velocity;
	float maxVelocity;
	float minVelocity;
	float acceleration;
	float friction;
	float gravity;
	float maxGravity;

	//Initialization functions
	void initVariables();
	void initSprite();
	void initAnmation();
	void initPhysics();
	
public:
	Player(std::string textureName, sf::IntRect sizeHitbox, sf::IntRect sizeSprite, sf::Vector2f position);
	virtual ~Player();

	void setScale(sf::Vector2f scale) //Entity.h
	{
		animatedSprite->setScale(scale);
		hitboxPlayer->setScale(scale);
	}

	Collider* getCollider() const //Entity.h
	{
		return hitboxPlayer.get();
	}

	//Accessors
	const sf::Vector2f getPosition() const; //Entity.h
	const sf::FloatRect getGlobalBounds() const; //Entity.h

	//Modifiers
	void setPosition(const float x, const float y); //Entity.h
	void resetVelocityY(); //Entity.h

	//Functions
	void move(const float x, const float y, float deltaTime); //Entity.h
	void updatePhysics(float deltaTime); //Entity.h
	void updateMovement(float deltaTime);
	void update(float deltaTime);  //Entity.h
	void render(sf::RenderTarget& target); //Entity.h
};

