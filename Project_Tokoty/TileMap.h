#pragma once
#include "Tile.h"
#include "Player.h"
//#include "Enemy.h"

class TileMap
{
private:
	sf::Vector2f playerSpawn;
	std::vector<sf::Vector2f> enemySpawn;

	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	std::vector< std::vector<std::optional<Tile>> > tilesArray;
	//std::vector< std::vector<std::unique_ptr<Tile>> > tilesArray;
	sf::Texture tilesetTexture; // trebuie sa coincida cu tileset-ul folosit in Tiled cand s-a creata fisierul .tmx
	sf::Vector2f scale; // scaleaza atat tile-urile cat si distanta dintre ele

	std::vector<std::vector<int>> tilesCoords; // coordonatele fiecarui tile (top-left corner)
	void initTilesetTexture(std::string textureName);
	void parseTMX(std::string tmxName);
public:
	TileMap(std::string tmxName, std::string textureName, sf::Vector2f scale = sf::Vector2f{1.0f, 1.0f});
	TileMap(std::string tmxName, sf::Texture& tileMapTexture, sf::Vector2f scale = sf::Vector2f{1.0f, 1.0f});
	~TileMap();

	void update(Entity* player);
	void render(sf::RenderTarget& window);

	sf::Vector2f getPlayerSpawn();
	std::vector<sf::Vector2f> getEnemySpawn();
};

