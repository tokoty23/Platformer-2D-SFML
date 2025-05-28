#include "stdafx.h"
#include "Collider.h"

Collider::Collider(sf::FloatRect hitbox, sf::Vector2f position, float push) :
	hitbox(hitbox)
{

}

Collider::Collider(sf::IntRect hitbox, sf::Vector2f position, float push) :
	Collider(sf::FloatRect(hitbox), position, push)
{

}

Collider::Collider(sf::FloatRect hitbox, float push) :
	hitbox(hitbox)
{

}

Collider::Collider(sf::IntRect hitbox, float push) :
	Collider(sf::FloatRect(hitbox), push)
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

void Collider::renderCollider(sf::RenderTarget& target, Collider& other)
{
	sf::Vector2f otherPosisiton = other.getPosition();
	sf::Vector2f otherHalfSize = other.getSize() / 2.0f;
	sf::Vector2f pos;
	pos.x = otherPosisiton.x + otherHalfSize.x;
	pos.y = otherPosisiton.y + otherHalfSize.y;
	sf::CircleShape circle;
	circle.setPosition(pos);
	circle.setFillColor(sf::Color(0, 255, 255, 128)); // Red, half transparent
	circle.setRadius(50.0f);
	target.draw(circle);
	sf::RectangleShape hitboxShape(sf::Vector2f(other.getSize()));

}

void Collider::renderCollider(sf::RenderTarget& target)
{
	
	sf::RectangleShape hitboxShape(sf::Vector2f(hitbox.size));
	hitboxShape.setPosition(hitbox.position);
	hitboxShape.setFillColor(sf::Color(0, 0, 255, 128)); // Red, half transparent
	target.draw(hitboxShape);
}

bool Collider::checkCollision(Collider& other, float push, sf::RenderTarget& target)
{
	sf::Vector2f otherPosisiton = other.getPosition();
	sf::Vector2f otherHalfSize = other.getSize() / 2.0f;
	sf::Vector2f thisPosisiton = getPosition();
	sf::Vector2f thisHalfSize = getSize() / 2.0f;

	float deltaX = (otherPosisiton.x + otherHalfSize.x) - (thisPosisiton.x + thisHalfSize.x);
	float deltaY = (otherPosisiton.y + otherHalfSize.y) - (thisPosisiton.y + thisHalfSize.y);

	float intersectX = std::abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
	float intersectY = std::abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);

	/*
	
	circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()) ); // Center the origin
	circle.setPosition(thisPosisiton + thisHalfSize); // Center on this collider
	target.draw(circle);
	circle.setPosition(otherPosisiton + otherHalfSize); // Center on this collider
	target.draw(circle);
	*/
	//std::cout << "Collision Player x" << otherPosisiton.x << "Collision Player y" << otherPosisiton.y << std::endl;

	
	if (intersectX < 0 && intersectY < 0)
	{
		std::cout << "Collision detected" << std::endl;
		sf::CircleShape circle;
		circle.setRadius(50.0f); // Use a smaller radius
		circle.setFillColor(sf::Color(0, 255, 255, 255));

		sf::RectangleShape hitboxShape(otherHalfSize + otherHalfSize );
		hitboxShape.setPosition(otherPosisiton + sf::Vector2f(10,10) );
		hitboxShape.setFillColor(sf::Color(0, 255, 0, 150)); // Red, half transparent
		target.draw(hitboxShape);

		sf::RectangleShape hitboxShape2(thisHalfSize + thisHalfSize);
		hitboxShape2.setPosition(thisPosisiton);
		hitboxShape2.setFillColor(sf::Color(0, 255, 0, 150)); // Red, half transparent
		target.draw(hitboxShape2);


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
