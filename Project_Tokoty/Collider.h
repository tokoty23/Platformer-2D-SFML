#pragma once
class Collider
{
private:
	sf::FloatRect hitbox;

public:

	Collider(sf::FloatRect hitbox, sf::Vector2f position, float push);
    Collider(sf::IntRect hitbox, sf::Vector2f position, float push);
	Collider(sf::FloatRect hitbox, float push);
	Collider(sf::IntRect hitbox, float push);
	virtual ~Collider();

	//Accessors
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	sf::FloatRect getGlobalBounds() const;
	void setSize(float width, float height) {
		hitbox.size.x = width;
		hitbox.size.y = height;
	}

	void setScale(sf::Vector2f scale) {
		hitbox.size.x *= scale.x;
		hitbox.size.y *= scale.y;
	}

	//Functions
	void move(float x, float y);
	void move(sf::Vector2f velocity);
	void setPosition(float x, float y);
	void renderCollider(sf::RenderTarget& target, Collider& other);
	void renderCollider(sf::RenderTarget& target);
	bool checkCollision(Collider& other, float push, sf::RenderTarget& target);

	

};

