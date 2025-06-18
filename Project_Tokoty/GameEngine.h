#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"
#include "Tile.h"
#include "TileMap.h"

class GameEngine
{
private:
	//Window
	sf::RenderWindow window;
	sf::Clock deltaClock;
	sf::View view;
	//sf::Event event;

	//Gameplay
	std::unique_ptr<Player> player;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::unique_ptr<TileMap> tileMap;
	
	//Initialization
	void initView();
	void initWindow();
	void initEntities();
	
	//
	void pollEvents();
	void updateEntities(float deltaTime);
	void updatePhysicsColliders();
	void updateCombatColliders();
	void updateView();

	void renderEntities();
	void renderTileMap();
public:
	GameEngine();
	virtual ~GameEngine();

	void run();
	//functions
	
	void update();
	void render();
	const sf::RenderWindow& getWindow() const;
};



