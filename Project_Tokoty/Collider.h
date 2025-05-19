#pragma once
class Collider
{
private:
	sf::FloatRect hitbox;
	sf::Vector2f position;
	sf::Vector2f size;
public:

	Collider(sf::FloatRect hitbox, float push);
	virtual ~Collider();
	//Accessors
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	//Functions
	void move(float x, float y);

	void renderCollider(sf::RenderTarget& target, Collider& other);
	bool checkCollision(Collider& other, float push);
};

