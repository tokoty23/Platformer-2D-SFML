#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap(std::string tmxName, std::string textureName, sf::Vector2f scale)
	:scale(scale)
{
	initTilesetTexture(textureName);
	parseTMX(tmxName);
}

TileMap::TileMap(std::string tmxName, sf::Texture& tileMapTexture, sf::Vector2f scale)
	:scale(scale)
{
	tilesetTexture = tileMapTexture;
	parseTMX(tmxName);
}

TileMap::~TileMap()
{
}

void TileMap::initTilesetTexture(std::string textureName)
{
	if (!tilesetTexture.loadFromFile("Textures/" + textureName))
	{
		std::cerr << "ERROR Class TileMap: initTilesetTexture(): " << textureName << std::endl;
		return;
	}
}


// \brief Parseaza fisierul TMX folosind libraria tmx
void TileMap::parseTMX(std::string tmxName)
{
	tmx::Map map;

	// nu stiu de ce merge doar cu \\ nu merge nici cu full path chiar daca folosesti "\\"
	if (!map.load("Maps\\" + tmxName))
	{
		std::cerr << "ERROR Class TileMap: parseTMX(): " << tmxName << std::endl;
	}

	mapSize = sf::Vector2i(map.getBounds().width, map.getBounds().height);
	tileSize = sf::Vector2i(map.getTileSize().x, map.getTileSize().y);

	const auto& layers = map.getLayers();
	
	for (const auto& layer : layers)
	{
		if (layer->getType() == tmx::Layer::Type::Tile)
		{
			auto* tile_layer = dynamic_cast<const tmx::TileLayer*>(layer.get());
			//if (!tile_layer) continue;
			const auto& tiles = tile_layer->getTiles();
			const auto& layer_size = tile_layer->getSize();
			const auto& tile_size = map.getTileSize();

			tilesArray.resize(layer_size.y);
			for (auto& row : tilesArray) {
				row.resize(layer_size.x);
			}
			
			int counter = 0;
			for (unsigned int y = 0; y < layer_size.y; ++y) {
				for (unsigned int x = 0; x < layer_size.x; ++x) {
					int index = x + y * layer_size.x;
					int tile_id = tiles[index].ID;
					
					
					if (tile_id == 0) {
						continue;
					}
					int tilesPerRow = tilesetTexture.getSize().x / tile_size.x;
					int tx = (tile_id - 1) % tilesPerRow;
					int ty = (tile_id - 1) / tilesPerRow;

					sf::IntRect tileRect({ tx * (int)tile_size.x, ty * (int)tile_size.y }, { (int)tile_size.x, (int)tile_size.y });

					counter++;
					std::cout << "Counter:" << counter << "Tile ID: " << tile_id << std::endl;
					//std::cout << "tiles.size(): " << tiles.size() << ", index: " << index << std::endl;
					sf::Vector2f worldPos(x * tile_size.x * scale.x, y * tile_size.y * scale.y);
					sf::Vector2i tile_size_i( (float)(tile_size.x), (float)(tile_size.y));
					//sf::IntRect tileRect({ 0, 0 }, tile_size_i);
					tilesArray[y][x].emplace(tilesetTexture, tileRect, tileRect, worldPos);
					tilesArray[y][x]->setScale(scale);
					if (tile_id == 1) 
					{
						

						//std::unique_ptr<Tile> tile = std::make_unique<Tile>(tilesetTexture, sf::IntRect({ 0, 0 }, tile_size_i), worldPos);
						//tile->setScale(scale);
						//tilesArray[y][x] = std::move(tile);
						//std::cout << index << std::endl;	
					}
					else if (tile_id == 2) 
					{
	
					}
					else if (tile_id == 3) 
					{
	
					}
					else {
						continue;
					}
				}
			}


		}

		else if (layer->getType() == tmx::Layer::Type::Object)
		{

			const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
			if (objectLayer.getName() == "Entities") {
				for (const auto& obj : objectLayer.getObjects()) {
					std::string className = obj.getType(); // Type este defapt Class in Tiled
					sf::Vector2f pos(obj.getPosition().x * scale.x, obj.getPosition().y * scale.y);

					if (className == "Player") {
						std::cout << "Player spawn position: " << pos.x << ", " << pos.y << std::endl;
						playerSpawn = pos;
					}
					else if (className == "Enemy") {
						enemySpawn.push_back(pos);
					}
				}
			}
		}
	}

}   




void TileMap::update(Entity* player)
{
	for (const auto& row : tilesArray) {
		for (const auto& tile : row) {
			if (tile) 
			{ 
				Collider* hitbox = player->getCollider(ColliderKeys::E_HITBOX);
				if (hitbox->checkCollision(*tile->getCollider(), 0.0f, true))
				{
					if (hitbox->intersectGround(*tile->getCollider()))
					{
						player->setIsGrounded(true);
						player->resetVelocityY();
					}
				}
					
				
					

				
			}
		}
	}
}

void TileMap::render(sf::RenderTarget& window)
{
	//std::cout << tilesArray[0][0].value().getCollider()->getPosition().x<<" x  y " << tilesArray[1][1].value().getCollider()->getPosition().y << std::endl;
	for (const auto& row : tilesArray) {
		for (const auto& tile : row) {
			if (tile) {
				tile->render(window);
			}
		}
	}
}

sf::Vector2f TileMap::getPlayerSpawn()
{
	return playerSpawn;
}

std::vector<sf::Vector2f> TileMap::getEnemySpawn()
{
	return enemySpawn;
}
