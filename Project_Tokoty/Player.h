#pragma once
#include "Collider.h"

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
	//sf::Sprite sprite;

	//Hitbox
	std::unique_ptr<Collider> hitboxPlayer;
	//Collider
	//Collider* hitboxPlayer = nullptr;
	
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
	Player();
	virtual ~Player();

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

