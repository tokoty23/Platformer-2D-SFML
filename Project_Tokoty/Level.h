#pragma once
#include "Player.h"
#include "Enemy.h"
#include "TileMap.h"
#include "Database.h"

class Level
{
private:
	std::unique_ptr<TileMap> tileMap;
	std::vector<std::unique_ptr<Enemy>> enemies;
	sf::Vector2f playerSpawn;
	std::vector<sf::Vector2f> enemySpawn;

	sf::Texture enemiesTexture;
	sf::Texture tilemapTexture;
	int currentLevel;


public:
	Level(std::string tmxName, sf::Texture& tileMaptexture, sf::Texture& enemiesTexture, sf::Vector2f scale);
	void loadLevel(std::string tmxName, sf::Texture& tileMapTexture, sf::Vector2f scale);

	void update(sf::Time deltaTime, Player* player);
	void render(sf::RenderTarget& target);

	void updatePhysicsColliders(Player* player);
	void updateCombatColliders(Player* player);

	sf::Vector2f getPlayerSpawn();
	std::vector<sf::Vector2f> getEnemySpawn();
	std::vector<bool> getEnemiesState();
	void respawnEnemy();
	//std::vector<std::unique_ptr<Enemy>> getEnemies();
	TileMap* getTileMap();
	int getCurrentLevel();
	std::vector<EnemySaveData> getEnemiesSaveData();
	void setEnemiesSaveData(std::vector<EnemySaveData> enemiesData);
};

