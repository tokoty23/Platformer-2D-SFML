#pragma once
class Collider
{
private:
	sf::FloatRect hitbox;
	sf::Vector2f position;
	sf::Vector2f size;
public:

	Collider(sf::Vector2f position, sf::Vector2f size, float push);
	virtual ~Collider();
	//Accessors
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	//Functions
	


	bool checkCollision(Collider& other, float push);
};

