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
	sf::FloatRect getGlobalBounds() const;
	//Functions
	void move(float x, float y);
	void move(sf::Vector2f velocity);
	void setPosition(float x, float y);
	void renderCollider(sf::RenderTarget& target, Collider& other);
	void renderCollider(sf::RenderTarget& target);
	bool checkCollision(Collider& other, float push, sf::RenderTarget& target);

	void setSize(float width, float height) {
		hitbox.size.x = width;
		hitbox.size.y = height;
	}

    bool checkCollision2(Collider& other, float push, sf::RenderTarget& target) {
        // Get bounds for both colliders
        sf::FloatRect thisBounds = getGlobalBounds();
        sf::FloatRect otherBounds = other.getGlobalBounds();

        // Calculate edges using position and size directly
        float leftThis = thisBounds.position.x;
        float rightThis = leftThis + thisBounds.size.x;
        float topThis = thisBounds.position.y;
        float bottomThis = topThis + thisBounds.size.y;

        float leftOther = otherBounds.position.x;
        float rightOther = leftOther + otherBounds.size.x;
        float topOther = otherBounds.position.y;
        float bottomOther = topOther + otherBounds.size.y;

        // Check for intersection
        bool collisionX = (rightThis > leftOther) && (leftThis < rightOther);
        bool collisionY = (bottomThis > topOther) && (topThis < bottomOther);

        sf::RectangleShape hitboxShape(sf::Vector2f(hitbox.size));
        hitboxShape.setPosition(hitbox.position+sf::Vector2f(10,10));
        hitboxShape.setFillColor(sf::Color(0, 255, 0, 150)); // Red, half transparent
        target.draw(hitboxShape);

        if (collisionX && collisionY) {
            // Calculate overlap depth
            float overlapX = (leftThis < leftOther)
                ? (rightThis - leftOther)
                : (rightOther - leftThis);

            float overlapY = (topThis < topOther)
                ? (bottomThis - topOther)
                : (bottomOther - topThis);

            // Resolve collision along smallest overlap axis
            if (std::abs(overlapX) < std::abs(overlapY)) {
                // Horizontal collision
                if (leftThis < leftOther) {
                    setPosition(getPosition().x - overlapX, getPosition().y);
                }
                else {
                    setPosition(getPosition().x + overlapX, getPosition().y);
                }
            }
            else {
                // Vertical collision
                if (topThis < topOther) {
                    setPosition(getPosition().x, getPosition().y - overlapY);
                }
                else {
                    setPosition(getPosition().x, getPosition().y + overlapY);
                }
            }
            return true;
        }
        return false;
    }
};

