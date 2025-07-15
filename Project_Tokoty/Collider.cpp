#include "stdafx.h"
#include "Collider.h"

Collider::Collider(sf::FloatRect hitbox, float push, ColliderType type) 
	: hitbox(hitbox), type(type)
{
	c_isActive = true;
}

Collider::Collider(sf::IntRect hitbox, float push, ColliderType type) 
	: Collider(sf::FloatRect(hitbox), push, type)
{

}



Collider::~Collider()
{
}

sf::Vector2f Collider::getPosition() const
{
	return hitbox.position;
}

sf::Vector2f Collider::getSize() const
{
	return hitbox.size;
}

sf::FloatRect Collider::getGlobalBounds() const
{
	return hitbox;
}

ColliderType Collider::getType() const
{
	return type;
}

void Collider::setActive(bool active)
{
	c_isActive = active;
}

void Collider::setSize(float width, float height) {
	hitbox.size.x = width;
	hitbox.size.y = height;
}

void Collider::setScale(sf::Vector2f scale) {
	hitbox.size.x *= scale.x;
	hitbox.size.y *= scale.y;
}

void Collider::move(float x, float y, sf::Time deltaTime)
{
	hitbox.position.x += x * deltaTime.asSeconds();
	hitbox.position.y += y * deltaTime.asSeconds();
}

void Collider::move(sf::Vector2f velocity, sf::Time deltaTime)
{
	hitbox.position.x += velocity.x * deltaTime.asSeconds();
	hitbox.position.y += velocity.y * deltaTime.asSeconds();
}

void Collider::move(float x, float y)
{
	hitbox.position.x += x;
	hitbox.position.y += y;
}

void Collider::move(sf::Vector2f velocity)
{
	hitbox.position.x += velocity.x;
	hitbox.position.y += velocity.y;
}

void Collider::setPosition(float x, float y)
{
	hitbox.position.x = x;
	hitbox.position.y = y;
}


bool Collider::isActive() const
{
	return c_isActive;
}

bool Collider::intersects(Collider& other) const
{
	if (!c_isActive || !other.isActive()) return false;
	sf::Vector2f otherPositon = other.getPosition();
	sf::Vector2f otherHalfSize = other.getSize() / 2.0f;
	sf::Vector2f thisPositon = getPosition();
	sf::Vector2f thisHalfSize = getSize() / 2.0f;

	float deltaX = (otherPositon.x + otherHalfSize.x) - (thisPositon.x + thisHalfSize.x);
	float deltaY = (otherPositon.y + otherHalfSize.y) - (thisPositon.y + thisHalfSize.y);

	float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
	float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);
	return (intersectX <= 0 && intersectY <= 0);

	/*
	if (!c_isActive || !other.isActive()) return false;
	// findIntersection returneaza un std::optional<sf::FloatRect> care este empty daca nu exista intersectie
	return this->hitbox.findIntersection(other.getGlobalBounds()).has_value();
	*/
}

bool Collider::intersectGround(Collider& other) const
{
	if (!c_isActive || !other.isActive()) return false;
	sf::Vector2f otherPositon = other.getPosition();
	sf::Vector2f otherHalfSize = other.getSize() / 2.0f;
	sf::Vector2f thisPositon = getPosition();
	sf::Vector2f thisHalfSize = getSize() / 2.0f;

	float deltaX = (otherPositon.x + otherHalfSize.x) - (thisPositon.x + thisHalfSize.x);
	float deltaY = (otherPositon.y + otherHalfSize.y) - (thisPositon.y + thisHalfSize.y);

	float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
	float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

	//std::cout << "thisPosition.x: " << thisPositon.x << ", otherPosition.x: " << otherPositon.x << std::endl;
	//std::cout << "thisPosition.y: " << thisPositon.y << ", otherPosition.y: " << otherPositon.y << std::endl;
	return (intersectX <= 0 && (thisPositon.y + 2 * thisHalfSize.y  <= otherPositon.y ) );
}


void Collider::renderCollider(sf::RenderTarget& target)
{
	if (!c_isActive) return;
	sf::RectangleShape hitboxShape(sf::Vector2f(hitbox.size));
	hitboxShape.setPosition(hitbox.position);

	switch (type)
	{
	case ColliderType::C_HITBOX:
		hitboxShape.setFillColor(sf::Color(0, 0, 255, 128)); // Red, half transparent
		break;
	case ColliderType::C_HURTBOX:
		hitboxShape.setFillColor(sf::Color(255, 255, 255, 128)); // Red, half transparent
		break;
	case ColliderType::C_ATTACKBOX:
		hitboxShape.setFillColor(sf::Color(255, 255, 0, 128)); // Green, half transparent
		break;
	case ColliderType::C_TRIGGERBOX:
		break;
	default:
		break;
	}

	target.draw(hitboxShape);
}

bool Collider::checkCollision(Collider& other, float push, bool t)
{
	if (!c_isActive || !other.isActive()) return false;
	sf::Vector2f otherPosisiton = other.getPosition();
	sf::Vector2f otherHalfSize = other.getSize() / 2.0f;
	sf::Vector2f thisPosisiton = getPosition();
	sf::Vector2f thisHalfSize = getSize() / 2.0f;

	float deltaX = (otherPosisiton.x + otherHalfSize.x) - (thisPosisiton.x + thisHalfSize.x);
	float deltaY = (otherPosisiton.y + otherHalfSize.y) - (thisPosisiton.y + thisHalfSize.y);

	float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
	float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

	if (intersectX > 0 || intersectY > 0)
	{
		return false;
	}
	else
	{
		push = std::clamp(push, 0.0f, 1.0f);

		if (intersectX > intersectY)
		{
			float dx = (deltaX > 0.f) ? intersectX : -intersectX;
			move(dx * (1.f - push), 0.f);
			other.move(-dx * push, 0.f);
		}
		else
		{
			float dy = (deltaY > 0.f) ? intersectY : -intersectY;
			move(0.f, dy * (1.f - push));
			other.move(0.f, -dy * push);
		}
		return true;
	}
}
