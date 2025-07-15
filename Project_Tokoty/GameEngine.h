#pragma once
#include "stdafx.h"
#include "Player.h"
#include "Enemy.h"
#include "Tile.h"
#include "TileMap.h"
#include "HUD.h"
#include "Level.h"
#include "Database.h"

//forward declarations
class State;
class MainMenuState;
class GameplayState;
class PauseMenuState;
class RespawnMenuState;

enum class GameState
{
	MAIN_MENU,
	GAMEPLAY,
	PAUSE_MENU,
	RESPAWN_MENU
};
class GameEngine
{
private:
	std::unique_ptr<Database> db;

	std::stack<std::unique_ptr<State>> states;

	sf::Texture playerSpritesheet;
	sf::Texture enemySpritesheet;
	sf::Texture tileMapSpritesheet;

	sf::Font font;

	//Window
	sf::RenderWindow window;
	sf::Clock deltaClock;
	sf::View view;
	std::unique_ptr<HUD> hud;
	//sf::Event event;

	//Gameplay
	std::unique_ptr<Player> player;
	std::unique_ptr<Level> level;
	
	//Initialization
	void initView();
	void initWindow();
	void initLevel();
	void initHUD();
	//
	void updateView();
public:
	GameEngine();
	virtual ~GameEngine();

	void runGameplay(sf::Time deltaTime);
	void run();
	//functions
	
	void update();
	void renderGameplay();
	sf::RenderWindow& getWindow();

	//State Stack
	void changeState(GameState newState);
	void pushState(GameState newState);
	void popState();

	//Database
	bool saveGame(int saveSlot);
	bool loadGame(int saveSlot);
	Player* getPlayer();
	void respawnEntities();

	const sf::View& getView();
};



