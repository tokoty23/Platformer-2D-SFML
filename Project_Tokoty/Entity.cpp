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
	attackDamage = 20.0f;

	facingRight = true;
	state = EntityState::E_IDLE;
	isDead = false;

	attackCooldownTimer.setDuration(sf::seconds(2.0f)); //timp de asteptare intre atacuri
	attackDurationTimer.setDuration(sf::seconds(0.7f)); //cat timp este activ attackbox-ul
	invincibilityTimer.setDuration(sf::seconds(1.0f)); //cat timp este inactiv hurtbox-ul dupa ce entity a fost atacat
	deathTimer.setDuration(sf::seconds(2.0f)); //cat timp entity este in starea EntityState::DYING inainte sa dispara
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

void Entity::updateTimers(sf::Time deltaTime)
{
	attackCooldownTimer.update(deltaTime);
	attackDurationTimer.update(deltaTime);
	invincibilityTimer.update(deltaTime);
	deathTimer.update(deltaTime);
	
	if (!attackDurationTimer.isActive())
	{
		Collider* attackBox = getCollider(ColliderKeys::E_ATTACKBOX_DEFAULT);
		if (attackBox)
		{
			attackBox->setActive(false);
		}
	}

	if (state == EntityState::E_DYING && !deathTimer.isActive())
	{
		setState(EntityState::E_DEAD);
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

void Entity::updatePhysics(sf::Time deltaTime)
{
	if (isDead) return;
	//Gravity
	if (!isGrounded)
	{
		velocity.y += 1.0f * gravity * deltaTime.asSeconds();
		if (std::abs(velocity.y) > maxGravity) velocity.y = maxGravity * ((velocity.y < 0) ? -1.0f : 1.0f);
	}
	
	velocity.y += 1.0f * gravity * deltaTime.asSeconds();
	if (std::abs(velocity.y) > maxGravity) velocity.y = maxGravity * ((velocity.y < 0) ? -1.0f : 1.0f);
	velocity.x *= friction;

	if (std::abs(velocity.x) < minVelocity * deltaTime.asSeconds()) velocity.x = 0.0f;
	if (std::abs(velocity.y) < minVelocity * deltaTime.asSeconds()) velocity.y = 0.0f;

	Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	if (collider) collider->move(velocity, deltaTime);

	/*
	if (!isGrounded)
	{
		if (velocity.y < 0) setState(EntityState::JUMPING);
		else if (velocity.y > 0) setState(EntityState::FALLING);
	}
	*/
}

void Entity::updateAnimation(sf::Time deltaTime)
{
	if (!animatedSprite) return;
	switch (state)
	{
	case EntityState::E_IDLE:
		if (facingRight)
			animatedSprite->playAnimation("IDLE", deltaTime);
		else
			animatedSprite->playAnimation("IDLE", deltaTime, true);

		break;
	case EntityState::E_MOVING:
			animatedSprite->playAnimation("MOVING", deltaTime, !facingRight);
		break;
	case EntityState::E_ATTACKING:
		if (facingRight)
			animatedSprite->playAnimation("ATTACK", deltaTime);
		else
			animatedSprite->playAnimation("ATTACK", deltaTime, true);
		break;

	case EntityState::E_DYING:
		animatedSprite->playAnimation("DYING", deltaTime, !facingRight);
		break;
	default:
		animatedSprite->playAnimation("IDLE", deltaTime);
		break;
	}
}

void Entity::move(const float x, const float y, sf::Time deltaTime)
{
	velocity.x += x * acceleration * deltaTime.asSeconds();
	velocity.y += y * acceleration * deltaTime.asSeconds();

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
				if (key == ColliderKeys::E_ATTACKBOX_DEFAULT)
				{
					std::cout << "Attackbox active at spawn: " << collider->isActive() << std::endl;
					collidersOffset[ColliderKeys::E_ATTACKBOX_DEFAULT_INITIAL] = offset;
				}
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

sf::Vector2f Entity::getColliderOffset(ColliderKeys key) const
{
	auto it = collidersOffset.find(key);
	if (it != collidersOffset.end())
	{
		return it->second;
	}
	return sf::Vector2f();
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

void Entity::setIsGrounded(bool grounded)
{
	isGrounded = grounded;
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
	std::cout << "Entity set position: " << x << ",  " << y << std::endl;
	std::cout << "Entity set position: " << x << ",  " << y << std::endl;
	std::cout << "Entity set position: " << x << ",  " << y << std::endl;
	std::cout << "Entity set position: " << x << ",  " << y << std::endl;
	std::cout << "Entity set position: " << x << ",  " << y << std::endl;
	std::cout << "Entity set position: " << x << ",  " << y << std::endl;
	std::cout << "Entity set position: " << x << ",  " << y << std::endl;
	std::cout << "Entity set position: " << x << ",  " << y << std::endl;
	Collider* collider = getCollider(ColliderKeys::E_HITBOX);
	if (collider)
	{
		collider->setPosition(x, y);
		syncCollidersWithHitbox();
	}
}

void Entity::setHealth(int health)
{
	this->health = health;
}

void Entity::setMaxHealth(int maxHealth)
{
	this->maxHealth = maxHealth;
}


void Entity::resetVelocityY()
{
	velocity.y = 0.0f;
}

// \brief updateaza fizica entitatii
void Entity::update(sf::Time deltaTime)
{
	if (state == EntityState::E_DEAD) return;
	updatePhysics(deltaTime);
	updateTimers(deltaTime);
	updateAnimation(deltaTime);
	//syncCollidersWithHitbox();
}

// \brief randeaza entitatea
void Entity::render(sf::RenderTarget& target)
{
	if (state == EntityState::E_DEAD) return;
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

void Entity::attack()
{
	//if (isDead) return;
	Collider* attackBox = getCollider(ColliderKeys::E_ATTACKBOX_DEFAULT);
	attackBox->setActive(true);
	attackDurationTimer.start();
}

void Entity::takeDamage(float damage)
{
	//if (isDead) return;
	health -= damage;
	if (health <= 0.0f)
	{
		health = 0.0f;
		die();
	}
	std::cout << "Entity took damage: " << damage << ", current health: " << health << std::endl;
}

void Entity::die()
{
	if (isDead) return;
	isDead = true;
	state = EntityState::E_DYING;
	velocity = sf::Vector2f(0.0f, 0.0f);
	for (const auto& collider : colliders)
	{
		collider.second->setActive(false);
	}
	deathTimer.start();
}

float Entity::getAttackDamage() const
{
	if (health > 0.0f)
	{
		return attackDamage;
	}
	return 0.0f;
}

float Entity::getHealth() const
{
	return health;
}

float Entity::getMaxHealth() const
{
	return maxHealth;
}

EntityState Entity::getState() const
{
	return state;
}

bool Entity::isDefeated()
{
	return isDead;
}

void Entity::setState(EntityState state)
{
	this->state = state;
}

void Entity::respawn(sf::Vector2f pos)
{
	setPosition(pos.x, pos.y);
	setState(EntityState::E_IDLE);
	isDead = false;
	health = maxHealth;
	velocity = sf::Vector2f(0, 0);
	deathTimer.stop();
	for (auto& collider : colliders)
	{
		collider.second->setActive(true);
	}
	syncCollidersWithHitbox();
	//colliders[ColliderType::C_HITBOX]
}
