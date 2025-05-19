#include "stdafx.h"
#include "Collider.h"

Collider::Collider(sf::FloatRect hitbox, float push) :
	hitbox(hitbox)
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

void Collider::move(float x, float y)
{
	hitbox.position.x += x;
	hitbox.position.y += y;
}

void Collider::renderCollider(sf::RenderTarget& target, Collider& other)
{
	sf::Vector2f otherPosisiton = other.getPosition();
	sf::Vector2f otherHalfSize = other.getSize() / 2.0f;
	sf::Vector2f pos;
	pos.x = otherPosisiton.x + otherHalfSize.x;
	pos.y = otherPosisiton.y + otherHalfSize.y;
	sf::CircleShape circle;
	circle.setPosition(pos);
	circle.setFillColor(sf::Color(255, 255, 255, 128)); // Red, half transparent
	circle.setRadius(50.0f);
	target.draw(circle);
}

bool Collider::checkCollision(Collider& other, float push)
{
	sf::Vector2f otherPosisiton = other.getPosition();
	sf::Vector2f otherHalfSize = other.getSize() / 2.0f;
	sf::Vector2f thisPosisiton = getPosition();
	sf::Vector2f thisHalfSize = getSize() / 2.0f;

	float deltaX = otherPosisiton.x - thisPosisiton.x;
	float deltaY = otherPosisiton.y - thisPosisiton.y;
	float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
	float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

	if (intersectX < 0 && intersectY < 0)
	{
		push = std::clamp(push, 0.0f, 1.0f);

		if (intersectX > intersectY)
		{
			if (deltaX > 0)
			{
				move(intersectX * (1.0f - push), 0.0f);
				other.move(-intersectX * push, 0.0f);
			}
			else
			{
				move(-intersectX * (1.0f - push), 0.0f);
				other.move(intersectX * push, 0.0f);
			}
		}
		else
		{
			if (deltaY > 0)
			{
				move(0.0f, intersectY * (1.0f - push));
				other.move(0.0f, -intersectY * push);
			}
			else
			{
				move(0.0f, -intersectY * (1.0f - push));
				other.move(0.0f, intersectY * push);
			}
		}
		return true;
	}

	return false;
}
