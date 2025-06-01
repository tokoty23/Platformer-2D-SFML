#pragma once
#include "Collider.h"
#include "UserInput.h"
#include "AnimatedSprite.h"

enum Player_AnimationStates
{
	IDLE = 0,
	LEFT,
	RIGHT,
	JUMP,
	FALL
};

class Player
{
private:
	//Sprite
	sf::Texture texture;
	std::optional<sf::Sprite> sprite;
	std::unique_ptr<AnimatedSprite> animatedSprite;
	//sf::Sprite sprite;

	//Hitbox
	std::unique_ptr<Collider> hitboxPlayer;
	//Collider
	//Collider* hitboxPlayer = nullptr;

	//User Input
	std::unique_ptr<UserInput> userInput;
	std::vector<UserInputData> userInputData;
	
	//Animation
	sf::IntRect currentFrame;
	sf::Clock animationClock;
	short animationState;

	//Physics
	sf::Vector2f velocity;
	float maxVelocity;
	float minVelocity;
	float acceleration;
	float friction;
	float gravity;
	float maxGravity;

	

	//Initialization functions
	void initVariables();
	void initTexture();
	void initSprite();
	void initAnmation();
	void initPhysics();
	
public:
	Player(std::string textureName, sf::IntRect currentRect, sf::Vector2f position);
	virtual ~Player();

	void setScale(sf::Vector2f scale)
	{
		animatedSprite->setScale(scale);
		hitboxPlayer->setScale(scale);
	}

	Collider* getCollider() const
	{
		return hitboxPlayer.get();
	}

	//Accessors
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;

	//Modifiers
	void setPosition(const float x, const float y);
	void resetVelocityY();

	//Functions
	void move(const float x, const float y);
	void updatePhysics();
	void updateMovement();
	void updateAnimation();
	void update();
	void render(sf::RenderTarget& target);
};

