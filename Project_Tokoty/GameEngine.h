#include "stdafx.h"
#include "Player.h"
#include "Tile.h"

class GameEngine
{
private:
	sf::RenderWindow window;
	//sf::Event event;
	Player* player;
	Tile* tile1;
	Tile* tile2;
	Tile* tile3;
	Tile* tile4;
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
	void updatePlayer();
	void renderPlayer();
	void update();
	void render();
	const sf::RenderWindow& getWindow() const;
};



