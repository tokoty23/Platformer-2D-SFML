#include "stdafx.h"
#include "Entity.h"


Entity::Entity(std::unique_ptr<Collider> hitbox, std::unique_ptr<AnimatedSprite> sprite)
	:  animatedSprite(std::move(sprite))
{
	if (hitbox)
	{
		colliders[ColliderKeys::E_HITBOX] = std::move(hitbox);
	}

;	initPhysics();
	initCombatStats();
	calculateSpriteOffset(false);
	std::cout << "spriteOffset: " << spriteOffset.x << " " << spriteOffset.y << std::endl;
}

void Entity::initPhysics()
{
	speed = 0.0f;
	velocity = sf::Vector2f(0.0f, 0.0f);
	maxVelocity = 0.0f;
	minVelocity = 0.0f;
	acceleration = 0.0f;
	friction = 0.0f;
	gravity = 0.0f;
	maxGravity = 0.0f;
	isGrounded = false;
}

void Entity::initCombatStats()
{
	maxHealth = 100.0f;
	health = maxHealth;
	attackDamage = 10.0f;

}

void Entity::calculateSpriteOffset(bool centered)
{
	Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	if (animatedSprite && collider)
	{
		sf::FloatRect bounds = animatedSprite->getGlobalBounds();
		spriteOffset.x = (collider->getSize().x - animatedSprite->getGlobalBounds().size.x) / 2.0f;
		//in functie de centered se alege daca offsetul sa fie impartit jumatate sus si jumatate jos sau doar complet sus
		spriteOffset.y = (collider->getSize().y - animatedSprite->getGlobalBounds().size.y) / (centered ? 2.0f : 1.0f);

	}
	else
	{
		spriteOffset = sf::Vector2f(0.f, 0.f);
	}
}

void Entity::syncCollidersWithHitbox()
{
	if (Collider* anchor = getCollider(ColliderKeys::E_HITBOX))
	{
		sf::Vector2f anchorPos = anchor->getPosition();

		// Iterăm prin offset-urile stocate
		for (const auto& pair : collidersOffset)
		{
			ColliderKeys key = pair.first;
			sf::Vector2f offset = pair.second;

			// Găsim satelitul corespunzător și îi setăm poziția corectă
			if (Collider* satellite = getCollider(key))
			{
				satellite->setPosition(anchorPos.x + offset.x, anchorPos.y + offset.y);
			}
		}
	}

}


void Entity::updatePhysics(float deltaTime)
{
	//Gravity
	velocity.y += 1.0f * gravity * deltaTime;
	if (std::abs(velocity.y) > maxGravity) velocity.y = maxGravity * ((velocity.y < 0) ? -1.0f : 1.0f);
	velocity *= friction;

	if (std::abs(velocity.x) < minVelocity) velocity.x = 0.0f;
	if (std::abs(velocity.y) < minVelocity) velocity.y = 0.0f;

	Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	if (collider) collider->move(velocity, deltaTime);
}

void Entity::move(const float x, const float y, float deltaTime)
{
	velocity.x += x * acceleration * deltaTime;
	velocity.y += y * acceleration * deltaTime;

	if (std::abs(velocity.x) > maxVelocity) velocity.x = maxVelocity * ((velocity.x < 0) ? -1.0f : 1.0f);
	//if (std::abs(velocity.y) > maxVelocity) velocity.y = maxVelocity * ((velocity.y < 0) ? -1.0f : 1.0f);

}

void Entity::addCollider(std::unique_ptr<Collider> collider, ColliderKeys key)
{
	if (collider)
	{
		// hitbox-ul este "ancora" lui entity, restul colliderelor sunt plasate cu un offset fata de hitbox
		if (key != ColliderKeys::E_HITBOX)
		{
			Collider* hitbox = getCollider(ColliderKeys::E_HITBOX);
			if (hitbox)
			{
				
				sf::Vector2f offset = collider->getPosition() - hitbox->getPosition();
				collidersOffset[key] = offset;
				std::cout << "collider added pos" << collider->getPosition().x << " " << collider->getPosition().y << std::endl;
				std::cout << "hitbox added pos" << hitbox->getPosition().x << " " << hitbox->getPosition().y << std::endl;
				std::cout << "offset" << offset.x << " " << offset.y<< std::endl;
			}
		}
		colliders[key] = std::move(collider);
	}
}

Collider* Entity::getCollider(ColliderKeys key) const
{
	//daca exista cheia in mapa returneaza un iterator la collider, altfel va returna .end()
	auto it = colliders.find(key);
	if (it != colliders.end())
	{
		return it->second.get();
	}
	return nullptr;
}

std::vector<Collider*> Entity::getColliders() const
{
	std::vector<Collider*> collidersList;
	for (const auto& pair : colliders)
	{
		collidersList.push_back(pair.second.get());
	}
	return collidersList;
}

void Entity::setScale(sf::Vector2f scale)
{
	for (auto& pair : colliders)
	{
		pair.second->setScale(scale);
	}
	for (auto& pair : collidersOffset)
	{
		pair.second.x *= scale.x;
		pair.second.y *= scale.y;
	}

	animatedSprite->setScale(scale);
	
	this->spriteOffset.x *= scale.x;
	this->spriteOffset.y *= scale.y;
}


const sf::Vector2f Entity::getPosition() const
{
	Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	if(collider) return collider->getPosition();

	return sf::Vector2f(0.0f, 0.0f);
		
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	if (collider) return collider->getGlobalBounds();
	
	return sf::FloatRect({ 0, 0 }, { 0, 0 });
}

void Entity::setPosition(const float x, const float y)
{
	Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	if (collider)
	{
		collider->setPosition(x, y);
		syncCollidersWithHitbox();
	}
}


void Entity::resetVelocityY()
{
	velocity.y = 0.0f;
}

// \brief updateaza fizica entitatii
void Entity::update(float deltaTime)
{
	updatePhysics(deltaTime);
	//syncCollidersWithHitbox();
}

// \brief randeaza entitatea
void Entity::render(sf::RenderTarget& target)
{
	Collider* collider = getCollider(ColliderKeys::E_HITBOX);

		if (animatedSprite && collider)
		{
			animatedSprite->setPosition(collider->getPosition() + spriteOffset);
			animatedSprite->render(target);
			for (const auto& pair : colliders)
			{
				pair.second->renderCollider(target);
			}
		}

}

void Entity::takeDamage(float damage)
{
	health -= damage;
	if (health <= 0.0f) health = 0.0f;
	std::cout << "Entity took damage: " << damage << ", current health: " << health << std::endl;
}

float Entity::getAttackDamage() const
{
	if (health > 0.0f)
	{
		return attackDamage;
	}
	return 0.0f;
}
