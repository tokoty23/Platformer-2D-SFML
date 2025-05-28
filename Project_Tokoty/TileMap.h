#pragma once
#include "Tile.h"

class TileMap
{
private:
	std::vector< std::vector<Tile*> > tiles;

public:
	TileMap();
	~TileMap();

	void addTile(int x, int y);
	void removeTile(int x, int y);

	void update();
	void render(sf::RenderTarget& target);
};

