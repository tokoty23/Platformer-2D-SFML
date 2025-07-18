#pragma once
#include "Collider.h"
#include "AnimatedSprite.h"
#include "Timer.h"

enum class ColliderKeys
{
	E_HITBOX,
	E_HURTBOX,
	E_ATTACKBOX_DEFAULT,
	E_ATTACKBOX_DEFAULT_INITIAL, // folosit pentru offset-ul initial inainte de .attack() care ar putea modifica offsetu-l in functie de atac
};

enum EntityState
{
	E_IDLE,
	E_MOVING,
	E_JUMPING,
	E_FALLING,
	E_ATTACKING,
	E_DYING,
	E_DEAD
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
	bool facingRight;
	bool isDead;

	//Combat Stats
	int maxHealth;
	int health;
	float attackDamage;
	//Timers
	Timer attackCooldownTimer;
	Timer attackDurationTimer;
	Timer invincibilityTimer;
	Timer deathTimer;

	EntityState state;

	void updatePhysics(sf::Time deltaTime);
	virtual void updateAnimation(sf::Time deltaTime);
	void move(const float x, const float y, sf::Time deltaTime);
	virtual void initPhysics();
	virtual void initCombatStats();
	virtual void calculateSpriteOffset(bool centered);
	virtual void updateTimers(sf::Time deltaTime);
	
public:
	Entity(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite);
	virtual ~Entity() = default;

	void addCollider(std::unique_ptr<Collider> collider, ColliderKeys key);
	Collider* getCollider(ColliderKeys key) const;
	sf::Vector2f getColliderOffset(ColliderKeys key) const;
	std::vector<Collider*> getColliders() const;

	void setScale(sf::Vector2f scale);
	virtual void setIsGrounded(bool grounded);

	//Accessors
	const sf::Vector2f getPosition() const;
	const sf::FloatRect getGlobalBounds() const;

	//Modifiers
	virtual void syncCollidersWithHitbox();
	void setPosition(const float x, const float y);
	void setHealth(int health);
	void setMaxHealth(int maxHealth);
	void resetVelocityY();
	virtual void update(sf::Time deltaTime);
	virtual void render(sf::RenderTarget& target);


	//Combat 
	virtual void attack();
	virtual void takeDamage(float damage);
	virtual void die();
	float getAttackDamage() const;
	virtual float getHealth() const;
	virtual float getMaxHealth() const;
	EntityState getState() const;
	bool isDefeated();
	void setState(EntityState state);
	void respawn(sf::Vector2f);


};