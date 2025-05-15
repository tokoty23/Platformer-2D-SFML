#include "stdafx.h"
#include "Collider.h"

Collider::Collider(sf::Vector2f position, sf::Vector2f size, float push)
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

	if (intersectX < 0.0f && intersectY < 0.0f)
	{
		if (intersectX > intersectY)
		{
			if (deltaX > 0.0f)
			{
				hitbox.position.x -= intersectX * push;
				other.hitbox.position.x += intersectX * (1.0f - push);
			}
			else
			{
				hitbox.position.x += intersectX * push;
				other.hitbox.position.x -= intersectX * (1.0f - push);
			}
		}
		else
		{
			if (deltaY > 0.0f)
			{
				hitbox.position.y -= intersectY * push;
				other.hitbox.position.y += intersectY * (1.0f - push);
			}
			else
			{
				hitbox.position.y += intersectY * push;
				other.hitbox.position.y -= intersectY * (1.0f - push);
			}
		}

		return true;
	}
	return false;
}
