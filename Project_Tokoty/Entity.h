#pragma once
#include "Collider.h"
#include "AnimatedSprite.h"

class Entity
{
protected:
	//Sprite
	std::unique_ptr<AnimatedSprite> animatedSprite;
	sf::Vector2f spriteOffset; //offset dintre coltul stanga sus al sprite-ului si collider-ul asociat

	//Hitbox
	std::unique_ptr<Collider> hitbox;

	//Physics
	float speed;
	sf::Vector2f velocity;
	float maxVelocity;
	float minVelocity;
	float acceleration;
	float friction;
	float gravity;
	float maxGravity;

	bool isGrounded;

	//Combat Stats

	void updatePhysics(float deltaTime);
	void move(const float x, const float y, float deltaTime);
	virtual void initPhysics();
	virtual void calculateSpriteOffset(bool centered);
public:
	Entity(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite);
	virtual ~Entity() = default;


	void setScale(sf::Vector2f scale);
	Collider* getCollider() const;

	//Accessors
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;

	//Modifiers
	void setPosition(const float x, const float y);
	void resetVelocityY();
	virtual void update(float deltaTime);
	virtual void render(sf::RenderTarget& target);
};