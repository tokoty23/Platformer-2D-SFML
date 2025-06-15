#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"
#include "Tile.h"
#include "TileMap.h"

class GameEngine
{
private:
	sf::RenderWindow window;
	sf::Clock deltaClock;
	//sf::Event event;
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	Tile* tile1;
	Tile* tile2;
	Tile* tile3;
	Tile* tile4;
	TileMap* tileMap;
	sf::View view;

	void initView();
	void initWindow();
	void initPlayer();
	void initTile();
public:
	GameEngine();
	virtual ~GameEngine();

	//functions
	void updateView();
	void updateCollisions();
	void updatePlayer(float deltaTime);
	void renderPlayer();
	void update();
	void render();
	const sf::RenderWindow& getWindow() const;
};



