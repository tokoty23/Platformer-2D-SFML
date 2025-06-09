#pragma once
#include "Tile.h"
#include "Player.h"

class TileMap
{
private:
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	std::vector<std::vector<int>> tilesCoords; // coordonatele fiecarui tile (top-left corner)
	std::vector< std::vector<std::unique_ptr<Tile>> > tilesArray;
	sf::Texture* tilesetTexture; // trebuie sa coincida cu tileset-ul folosit in Tiled cand s-a creata fisierul .tmx
	sf::Vector2f scale; // scaleaza atat tile-urile cat si distanta dintre ele
	void initTilesetTexture(std::string textureName);
	void parseTMX(std::string tmxName);
public:
	TileMap(std::string tmxName, std::string textureName, sf::Vector2f scale = sf::Vector2f{1.0f, 1.0f});
	~TileMap();

	void update(Player* player);
	void render(sf::RenderTarget& window);
};

