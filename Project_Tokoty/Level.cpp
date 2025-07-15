#include "stdafx.h"
#include "Level.h"

Level::Level(std::string tmxName, sf::Texture& tileMapTexture, sf::Texture& enemiesTexture, sf::Vector2f scale)
	: tilemapTexture(tileMapTexture), enemiesTexture(enemiesTexture)
{
	currentLevel = 1;
	loadLevel(tmxName, tileMapTexture, scale);
}

void Level::loadLevel(std::string tmxName, sf::Texture& tileMapTexture, sf::Vector2f scale)
{
	// "Grass_sheet_v1.png"
	tileMap = std::make_unique<TileMap>(tmxName, tilemapTexture, sf::Vector2f(2.0f, 2.0f));
	
	playerSpawn = tileMap->getPlayerSpawn();
	enemySpawn = tileMap->getEnemySpawn();

	sf::IntRect sizeSprite = sf::IntRect({ 0, 0 }, { 48, 48 });
	for (const auto& enemy : enemySpawn)
	{
		sf::FloatRect enemySizeHitbox = sf::FloatRect(enemy, { 48 - 28, 48 - 12 });
		auto enemy1 = std::make_unique<Enemy>(
			std::make_unique<Collider>(enemySizeHitbox, 0.5),
			std::make_unique<AnimatedSprite>(enemiesTexture, sizeSprite)
		);
		enemy1->setScale({ 2.0f, 2.0f });
		enemies.push_back(std::move(enemy1));
	}


}

void Level::update(sf::Time deltaTime, Player* player)
{
	for (auto& enemy : enemies)
	{
		enemy->update(deltaTime);
	}
	updatePhysicsColliders(player);
	player->syncCollidersWithHitbox();
	for (auto& enemy : enemies)
	{
		enemy->syncCollidersWithHitbox();
	}
	updateCombatColliders(player);
}

void Level::render(sf::RenderTarget& target)
{
	tileMap->render(target);
	for (const auto& enemy : enemies)
	{
		enemy->render(target);
	}
}

void Level::updatePhysicsColliders(Player* player)
{
	player->setIsGrounded(false);

	tileMap->update(player);
	for (auto& enemy : enemies)
	{
		enemy->setIsGrounded(false);
		tileMap->update(enemy.get());
	}


	Collider* playerHitbox = player->getCollider(ColliderKeys::E_HITBOX);
	if (playerHitbox)
	{
		for (auto& enemy : enemies)
		{
			Collider* enemyHitbox = enemy->getCollider(ColliderKeys::E_HITBOX);
			if (enemyHitbox)
			{
				playerHitbox->checkCollision(*enemyHitbox, 0.5f);
				if (player->getCollider(ColliderKeys::E_HITBOX)->intersectGround(*enemyHitbox))
				{
					//std::cout << "Player hitbox intersects with enemy hitbox!" << std::endl;
					//player->setIsGrounded(true);
					//player->resetVelocityY();
				}
			}
		}
	}
}

void Level::updateCombatColliders(Player* player)
{
	Collider* playerAttackbox = player->getCollider(ColliderKeys::E_ATTACKBOX_DEFAULT);
	if (playerAttackbox && playerAttackbox->isActive())
	{
		for (auto& enemy : enemies)
		{

			Collider* enemyHurtbox = enemy->getCollider(ColliderKeys::E_HURTBOX);
			if (enemyHurtbox && enemyHurtbox->isActive() && playerAttackbox->intersects(*enemyHurtbox))
			{
				playerAttackbox->setActive(false);
				enemy->takeDamage(player->getAttackDamage());
			}
		}
	}

	Collider* playerHurtbox = player->getCollider(ColliderKeys::E_HURTBOX);
	if (playerHurtbox && playerHurtbox->isActive())
	{
		for (auto& enemy : enemies)
		{
			Collider* enemyAttackbox = enemy->getCollider(ColliderKeys::E_ATTACKBOX_DEFAULT);
			//std::cout << "Checking player1 hurtbox: " << enemyAttackbox->isActive() << std::endl;
			//std::cout << "Checking player2 hurtbox: " << playerHurtbox->intersects(*enemyAttackbox) << std::endl;
			//std::cout << "Checking player3 hurtbox: " << enemyAttackbox << std::endl;
			if (enemyAttackbox && enemyAttackbox->isActive() && playerHurtbox->intersects(*enemyAttackbox))
			{
				std::cout << "Player hit by enemy attack!" << std::endl;
				enemyAttackbox->setActive(false);
				player->takeDamage(enemy->getAttackDamage());
			}
		}
	}
}


//
sf::Vector2f Level::getPlayerSpawn()
{
	return playerSpawn;
}

void Level::respawnEnemy()
{
	for (int i = 0; i < enemies.size() && i < tileMap->getEnemySpawn().size(); i++)
	{
		enemies[i]->respawn(enemySpawn[i]);
	}
}


std::vector<sf::Vector2f> Level::getEnemySpawn()
{
	return enemySpawn;
}

std::vector<bool> Level::getEnemiesState()
{
	std::vector<bool> enemyStates;
	for (const auto& enemy : enemies) {
		enemyStates.push_back(enemy->isDefeated());
	}
	return enemyStates;
}

TileMap* Level::getTileMap()
{
	return tileMap.get();
}

int Level::getCurrentLevel()
{
	return currentLevel;
}

std::vector<EnemySaveData> Level::getEnemiesSaveData()
{
	std::vector<EnemySaveData> enemiesData;
	for (int i = 0; i < enemies.size(); i++)
	{
		EnemySaveData data;
		data.enemy_id = i;
		data.health = enemies[i]->getHealth();
		data.is_defeated = enemies[i]->isDefeated();
		data.x = enemies[i]->getPosition().x;
		data.y = enemies[i]->getPosition().y;

		enemiesData.push_back(data);
	}

	return enemiesData;
}

void Level::setEnemiesSaveData(std::vector<EnemySaveData> enemiesData)
{
	{
		enemies.clear(); // Remove any existing enemies

		// Define sprite size for enemy animation
		sf::IntRect sizeSprite = sf::IntRect({ 0, 0 }, { 48, 48 });

		for (const auto& data : enemiesData)
		{
			if (data.is_defeated) continue;

			// Define hitbox size and position
			sf::FloatRect enemyHitbox({ data.x, data.y }, { 48 - 28, 48 - 12 });

			// Create enemy with collider and sprite
			auto enemy = std::make_unique<Enemy>(
				std::make_unique<Collider>(enemyHitbox, 0.5f),
				std::make_unique<AnimatedSprite>(enemiesTexture, sizeSprite)
			);

			// Restore saved properties
			enemy->setHealth(data.health);
			enemy->setState(data.is_defeated ? EntityState::E_IDLE : EntityState::E_DEAD);
			enemy->setScale({ 2.0f, 2.0f });
			std::cout << " data.y = " << data.y << std::endl;
			enemy->setPosition(data.x, data.y);	
			
			
			enemies.push_back(std::move(enemy));

			
		}
	}
}
