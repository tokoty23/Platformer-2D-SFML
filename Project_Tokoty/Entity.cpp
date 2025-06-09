#include "stdafx.h"
#include "Entity.h"

void Entity::initPhysics()
{
	speed = 0.0f;
	velocity = sf::Vector2f(0.0f, 0.0f);
	maxVelocity = 0.0f;
	minVelocity = 0.0f;
	acceleration = 0.0f;
	friction = 0.0f;
	gravity = 0.0f;
	maxGravity = 0.0f;
	isGrounded = false;
}

Entity::Entity(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite)
	: hitbox(std::move(hitbox)), animatedSprite(std::move(sprite))
{
	initPhysics();
}

void Entity::updatePhysics(float deltaTime)
{
	//Gravity
	velocity.y += 1.0f * gravity * deltaTime;
	if (std::abs(velocity.y) > maxGravity) velocity.y = maxGravity * ((velocity.y < 0) ? -1.0f : 1.0f);
	std::cout << "Velocity Y: " << velocity.y << std::endl;
	velocity *= friction;

	if (std::abs(velocity.x) < minVelocity) velocity.x = 0.0f;
	if (std::abs(velocity.y) < minVelocity) velocity.y = 0.0f;

	if(hitbox) hitbox->move(velocity, deltaTime);
	
}

void Entity::move(const float x, const float y, float deltaTime)
{
	velocity.x += x * acceleration * deltaTime;
	velocity.y += y * acceleration * deltaTime;

	if (std::abs(velocity.x) > maxVelocity) velocity.x = maxVelocity * ((velocity.x < 0) ? -1.0f : 1.0f);
	//if (std::abs(velocity.y) > maxVelocity) velocity.y = maxVelocity * ((velocity.y < 0) ? -1.0f : 1.0f);

}

void Entity::setScale(sf::Vector2f scale)
{
	animatedSprite->setScale(scale);
	hitbox->setScale(scale);
}

Collider* Entity::getCollider() const 
{
	return hitbox.get();
}

const sf::Vector2f Entity::getPosition() const
{
	return hitbox->getPosition();
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	return hitbox->getGlobalBounds();
}

void Entity::setPosition(const float x, const float y)
{
	hitbox->setPosition(x, y);
}

void Entity::resetVelocityY()
{
	velocity.y = 0.0f;
}

void Entity::update(float deltaTime)
{
	updatePhysics(deltaTime);
}

void Entity::render(sf::RenderTarget& target)
{
	if (animatedSprite)
	{
		animatedSprite->setPosition(hitbox->getPosition());
		animatedSprite->render(target);
	}
	hitbox->renderCollider(target);
}

