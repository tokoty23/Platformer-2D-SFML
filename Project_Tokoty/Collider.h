#pragma once

enum class ColliderType
{
	C_HITBOX,			 // collider pentru coliziunea intre obiecte
	C_HURTBOX,			 // collider unde entitatea poate primi daune; hurtbox intersecteaz attackbox -> pierzi HP  
	C_ATTACKBOX, 		 // collider pentru a ataca alte entitati; attackbox intersecteaza hurtbox -> inamicul pierde HP
	C_HITBOX_HURTBOX,    // hitbox_hurtbox combina hitbox si hurtbox pentru a nu fi nevoie de 2 collider-uri separate daca au aceleasi dimensiuni 
	C_TRIGGERBOX		 // collider care declanseaza un eveniment
};


class Collider
{
private:
	sf::FloatRect hitbox;
	ColliderType type;

public:

	Collider(sf::FloatRect hitbox, sf::Vector2f position, float push, ColliderType type = ColliderType::C_HITBOX);
    Collider(sf::IntRect hitbox, sf::Vector2f position, float push, ColliderType type = ColliderType::C_HITBOX);
	virtual ~Collider();

	//Getters
	sf::Vector2f getPosition() const;
	sf::Vector2f getSize() const;
	sf::FloatRect getGlobalBounds() const;
	ColliderType getType() const;
	//Setters
	void setSize(float width, float height);
	void setScale(sf::Vector2f scale);
	void setPosition(float x, float y);

	//Functions
	void move(float x, float y, float deltaTime);
	void move(sf::Vector2f velocity, float deltaTime);
	void move(float x, float y);
	void move(sf::Vector2f velocity);
	
	bool intersects(Collider& other) const;
	void renderCollider(sf::RenderTarget& target);
	bool checkCollision(Collider& other, float push);	

};

