#pragma once
#include "Collider.h"
#include "AnimatedSprite.h"

enum class ColliderKeys
{
	E_HITBOX,
	E_HURTBOX,
	E_ATTACKBOX_DEFAULT
};

class Entity
{
protected:
	//Sprite
	std::unique_ptr<AnimatedSprite> animatedSprite;
	sf::Vector2f spriteOffset; //offset dintre coltul stanga sus al sprite-ului si collider-ul asociat

	//Hitbox
	std::map<ColliderKeys, std::unique_ptr<Collider>> colliders;
	std::map<ColliderKeys, sf::Vector2f> collidersOffset;

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
	int maxHealth;
	int health;
	float attackDamage;

	void updatePhysics(float deltaTime);
	void move(const float x, const float y, float deltaTime);
	virtual void initPhysics();
	virtual void initCombatStats();
	virtual void calculateSpriteOffset(bool centered);
	
public:
	Entity(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite);
	virtual ~Entity() = default;

	void addCollider(std::unique_ptr<Collider> collider, ColliderKeys key);
	Collider* getCollider(ColliderKeys key) const;
	std::vector<Collider*> getColliders() const;

	void setScale(sf::Vector2f scale);

	//Accessors
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;

	//Modifiers
	virtual void syncCollidersWithHitbox();
	void setPosition(const float x, const float y);
	void resetVelocityY();
	virtual void update(float deltaTime);
	virtual void render(sf::RenderTarget& target);

	//Combat 
	void takeDamage(float damage);
	float getAttackDamage() const;
};