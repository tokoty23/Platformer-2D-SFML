#pragma once

enum class ColliderType
{
	C_HITBOX,			 // collider pentru coliziunea intre obiecte
	C_HURTBOX,			 // collider unde entitatea poate primi daune; hurtbox intersecteaz attackbox -> pierzi HP  
	C_ATTACKBOX, 		 // collider pentru a ataca alte entitati; attackbox intersecteaza hurtbox -> inamicul pierde HP 
	C_TRIGGERBOX		 // collider care declanseaza un eveniment
};


class Collider
{
private:
	sf::FloatRect hitbox;
	ColliderType type;
	bool c_isActive;

public:

	Collider(sf::FloatRect hitbox, float push = 0.0f, ColliderType type = ColliderType::C_HITBOX);
    Collider(sf::IntRect hitbox, float push = 0.0f, ColliderType type = ColliderType::C_HITBOX);
	virtual ~Collider();



	//Getters
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	sf::FloatRect getGlobalBounds() const;
	ColliderType getType() const;
	//Setters
	void setActive(bool active);
	void setSize(float width, float height);
	void setScale(sf::Vector2f scale);
	void setPosition(float x, float y);

	//Functions
	void move(float x, float y, sf::Time deltaTime);
	void move(sf::Vector2f velocity, sf::Time deltaTime);
	void move(float x, float y);
	void move(sf::Vector2f velocity);
	
	bool isActive() const;
	bool intersects(Collider& other) const;
	bool intersectGround(Collider&  other) const;
	void renderCollider(sf::RenderTarget& target);
	bool checkCollision(Collider& other, float push, bool t = false);	

};

