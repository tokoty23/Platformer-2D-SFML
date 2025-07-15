#include "stdafx.h"
#include "GameEngine.h"
#include "State.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "PauseMenuState.h"
#include "RespawnMenuState.h"

GameEngine::GameEngine()
{
	if (!font.openFromFile("Fonts/Munro.ttf"))
	{
		std::cerr << "ERROR class HUD: constructor()" << std::endl;
	}
	
	db = std::make_unique<Database>("savefile.db");
	db->initializeDatabase();

	initWindow();
	initLevel();
	//initView();
	initHUD();
	pushState(GameState::MAIN_MENU);
}


GameEngine::~GameEngine()
{
	//delete player;
}

void GameEngine::initWindow()
{
	window.create(sf::VideoMode({ 800,600 }), "Project Tokoty", sf::Style::Close | sf::Style::Titlebar); //VideoMode::getDesktopMode()
	//window.create(sf::VideoMode::getDesktopMode(), "Project Tokoty", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(1000); // customize option to be added
}

void GameEngine::initView()
{
	sf::Vector2f plyayerCenter = player->getPosition();
	plyayerCenter.x += player->getGlobalBounds().size.x / 2;
	plyayerCenter.y += player->getGlobalBounds().size.y / 2 + window.getSize().y / (float)(4);
	view.setCenter(plyayerCenter);
	window.setView(view);
}

void GameEngine::initLevel()
{
	if (!tileMapSpritesheet.loadFromFile("Textures/Grass_sheet_v1.png")) {
		throw std::runtime_error("Failed to load map texture!");
	}
	if (!playerSpritesheet.loadFromFile("Textures/SPRITESHEET_1_mod.png")) {
		throw std::runtime_error("Failed to load player texture!");
	}
	enemySpritesheet = playerSpritesheet;
	level = std::make_unique<Level>("level1.tmx", tileMapSpritesheet, enemySpritesheet, sf::Vector2f(2.0f, 2.0f) );

	sf::Vector2f playerSpawn = level->getPlayerSpawn();
	//std::cout << "Player spawn position: " << playerSpawn.x << ", " << playerSpawn.y << std::endl;
	
	sf::IntRect sizeSprite = sf::IntRect({ 0, 0 }, { 48, 48 }); // Primul cadru pentru sprite
	sf::FloatRect sizeHitbox = sf::FloatRect(playerSpawn, {48 - 28, 48 - 12}); // Dimensiunea pentru hitbox

	player = std::make_unique<Player>(
		std::make_unique<Collider>(sizeHitbox, 0.5),
		//std::make_unique<AnimatedSprite>(texture, sizeSprite, sf::Vector2f{ (float)sizeHitbox.position.x, (float)sizeHitbox.position.y })
		std::make_unique<AnimatedSprite>(playerSpritesheet, sizeSprite)
	);
	player->setScale({ 3.0f, 3.0f });


}

void GameEngine::initHUD()
{
	hud = std::make_unique<HUD>(window, player.get(), font);
}

void GameEngine::runGameplay(sf::Time deltaTime)
{
	player->update(deltaTime);
	
	level->update(deltaTime, player.get());
	
	hud->update(deltaTime);
}

void GameEngine::run()
{
	// restartam clock-ul dupa initializari deoarece incarcarea hartii dureaza mult timp
	// poate duce la un deltaTime foarte mare pentru un frame
	deltaClock.restart();
	//window.setView(window.getDefaultView()); //trebuie pus daca este folosit initWindow()
	while (window.isOpen())
	{
		// .restart() returneaza timpul de la ultimul frame inainte de a-l reseta
		sf::Time deltaTime = deltaClock.restart();
		deltaTime = std::min(deltaTime, sf::seconds(1.0f / 15.0f)); // limitam la 15 fps 
		//runGameplay(deltaTime);
		if (states.empty()) {
			window.close();
			continue;
		}
		if (!states.empty())
		{
			while (std::optional<sf::Event> event = window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
				{
					window.close();
				}
				else if (const auto* resized = event->getIf<sf::Event::Resized>())
				{
					std::cout << "test" << std::endl << std::endl << std::endl << std::endl << std::endl;
				}
				else
				{
					//std::cout << "Processing event, stack size: " << states.size() << std::endl;
					states.top()->handleInput(*this, event);
				}	
				if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
					if (keyPressed->scancode == sf::Keyboard::Scancode::P)
					{
						saveGame(1);
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::O)
					{
						loadGame(1);
					}
					else if (keyPressed->scancode == sf::Keyboard::Scancode::Tab)
					{
						window.setSize(sf::Vector2u(1080, 1920) );
						window.create(sf::VideoMode::getDesktopMode(), "Project Tokoty", sf::Style::Default | sf::Style::Titlebar, sf::State::Fullscreen);
						//view.setSize(sf::Vector2f(1080, 1920));
						
						if (!states.empty())
						{
							states.top()->resizeWindow(window);
						}
					}

				}
			}
		}
		if (!states.empty())
		{
			//std::cout << "Updating state, stack size: " << states.size() << std::endl;
			states.top()->update(*this, deltaTime);
		}
		if (!states.empty())
		{
			//std::cout << "Rendering state, stack size: " << states.size() << std::endl;

			window.clear();
			states.top()->render(window);
			window.display();
		}
		
	}

}


void GameEngine::updateView()
{
	sf::Vector2f playerCenter = player->getPosition();
	playerCenter.x += player->getGlobalBounds().size.x / 2;
	playerCenter.y += player->getGlobalBounds().size.y / 2 - window.getSize().y / (float)(4);


	sf::Vector2f mapSizePixels = {
		30 * 48.f * 2.f,
		20 * 48.f * 2.f
	};
	sf::Vector2f halfViewSize = view.getSize() / 2.f;
	if (playerCenter.x < halfViewSize.x)
		playerCenter.x = halfViewSize.x;
	else if (playerCenter.x > mapSizePixels.x - halfViewSize.x)
		playerCenter.x = mapSizePixels.x - halfViewSize.x;


	if (playerCenter.y < halfViewSize.y)
		playerCenter.y = halfViewSize.y;
	else if (playerCenter.y > mapSizePixels.y - halfViewSize.y)
		playerCenter.y = mapSizePixels.y - halfViewSize.y;


	//playerCenter.y += player->getGlobalBounds().size.y / 2 ;
	view.setCenter(playerCenter);
	window.setView(view);
}


void GameEngine::renderGameplay()
{
	updateView();
	level->render(window);
	player->render(window);

	window.setView(window.getDefaultView());
	hud->render(window);
	updateView();

}

sf::RenderWindow& GameEngine::getWindow()
{
	return this->window;
}

void GameEngine::changeState(GameState state)
{
	popState();
	pushState(state);
}

void GameEngine::pushState(GameState state)
{
	switch (state)
	{
	case GameState::MAIN_MENU:
		states.push(std::make_unique<MainMenuState>(*this, font));
		break;
	case GameState::GAMEPLAY:
		states.push(std::make_unique<GameplayState>(*this, font));
		break;
	case GameState::PAUSE_MENU:
		states.push(std::make_unique<PauseMenuState>(*this, font));
		break;
	case GameState::RESPAWN_MENU:
		states.push(std::make_unique<RespawnMenuState>(*this, font));
		break;
	}

}

void GameEngine::popState()
{
	if (!states.empty()) {
		states.pop();
	}
}

bool GameEngine::saveGame(int saveSlot)
{
	if (!player || !level) return false;

	PlayerSaveData playerData;
	playerData.health = player->getHealth();
	playerData.maxHealth = player->getMaxHealth();
	playerData.x = player->getPosition().x;
	playerData.y = player->getPosition().y;

	std::vector<EnemySaveData> enemiesData = level->getEnemiesSaveData();

	int currentLevel = level->getCurrentLevel();

	return db->saveGame(saveSlot, currentLevel, playerData, enemiesData);

	
}

bool GameEngine::loadGame(int saveSlot)
{
	int currentLevel;
	PlayerSaveData playerData;
	std::vector<EnemySaveData> enemiesData;
	std::cout << "LOAD GAME" << std::endl << std::endl << std::endl << std::endl << std::endl;
	if (!db->loadGame(saveSlot, currentLevel, playerData, enemiesData)) {
		std::cout << " 0" << std::endl;
		return false;
	}
	std::cout << "LOAD GAME" << std::endl << std::endl << std::endl << std::endl << std::endl;
	std::string tmxName = "level" + std::to_string(currentLevel) + ".tmx";
	level = std::make_unique<Level>(tmxName, tileMapSpritesheet, enemySpritesheet, sf::Vector2f(2.0f, 2.0f));


	player->setHealth(playerData.health);
	player->setMaxHealth(playerData.maxHealth);
	player->setPosition(playerData.x, playerData.y);
	player->syncCollidersWithHitbox();

	level->setEnemiesSaveData(enemiesData);

	return true;
}

Player* GameEngine::getPlayer()
{
	if (player) return player.get();
	return nullptr;
	
}

void GameEngine::respawnEntities()
{
	player->respawn(level->getPlayerSpawn());
	level->respawnEnemy();
}

const sf::View& GameEngine::getView()
{
	return view;
}
