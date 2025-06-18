#include "stdafx.h"
#include "TileMap.h"


void TileMap::initTilesetTexture(std::string textureName)
{
	tilesetTexture = new sf::Texture();
	if (!tilesetTexture->loadFromFile("Textures/" + textureName))
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
			const auto& tiles = tile_layer->getTiles();
			const auto& layer_size = tile_layer->getSize();
			const auto& tile_size = map.getTileSize();

			tilesArray.resize(layer_size.y);
			for (auto& row : tilesArray) {
				row.resize(layer_size.x);
			}
			
			int counter = 0;
			for (int y = 0; y < layer_size.y; ++y) {
				for (int x = 0; x < layer_size.x; ++x) {
					int index = x + y * layer_size.x;
					int tile_id = tiles[index].ID;
					
					
					if (tile_id == 0) {
						continue;
					}

					
					if (tile_id == 1) {
						counter++;
						std::cout << "Counter:" << counter << "Tile ID: " << tile_id << std::endl;
						//std::cout << "tiles.size(): " << tiles.size() << ", index: " << index << std::endl;
						sf::Vector2f worldPos(x * tile_size.x * scale.x, y * tile_size.y * scale.y);
						sf::Vector2i tile_size_i(static_cast<float>(tile_size.x), static_cast<float>(tile_size.y));
						
						std::unique_ptr<Tile> tile = std::make_unique<Tile>(tilesetTexture, sf::IntRect({ 0, 0 }, tile_size_i), sf::IntRect({ 0, 0 }, tile_size_i), worldPos);
						tile->setScale(scale );
						tilesArray[y][x] = std::move(tile);
						//std::cout << index << std::endl;
						
					}
					else if (tile_id == 2) {
						std::cout << index << std::endl;
					}
					else {
						continue;
					}
				}
			}


		}
	}

}   

TileMap::TileMap(std::string tmxName, std::string textureName, sf::Vector2f scale)
	:scale(scale)
{
	initTilesetTexture(textureName);
	parseTMX(tmxName);
}

TileMap::~TileMap()
{
}


void TileMap::update(Entity* player)
{
	for (const auto& row : tilesArray) {
		for (const auto& tile : row) {
			if (tile) {
				if (player->getCollider(ColliderKeys::E_HITBOX)->intersects(*tile->getCollider())) //std::cout << "Collider::intersects() not implemented yet" << std::endl;
				player->getCollider(ColliderKeys::E_HITBOX)->checkCollision(*tile->getCollider(), 0.0f);

				
			}
		}
	}
}

void TileMap::render(sf::RenderTarget& window)
{
	for (const auto& row : tilesArray) {
		for (const auto& tile : row) {
			if (tile) {
				tile->render(window);
			}
		}
	}
}