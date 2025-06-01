#include "stdafx.h"
#include "GameEngine.h"

void GameEngine::initView()
{
	sf::Vector2f plyayerCenter = player->getPosition();
	plyayerCenter.x += player->getGlobalBounds().size.x / 2;
	plyayerCenter.y += player->getGlobalBounds().size.y / 2;
	view.setCenter(plyayerCenter);
	window.setView(view);
}

void GameEngine::initWindow()
{
	window.create(sf::VideoMode({800,600}), "Project Tokoty", sf::Style::Close | sf::Style::Titlebar); //VideoMode::getDesktopMode()
	window.setFramerateLimit(60); // customize option to be added
}

void GameEngine::initPlayer()
{
	player = new Player("Soldier.png", sf::IntRect({ 0, 0 }, { 100, 100 }), { 0, 50 } );
	player->setScale({ 2.0f, 2.0f });
}

void GameEngine::initTile()
{
	// tiles are ca parametri 1.imaginea 2.intrect pentru marimea hitbox, 3.pentru marimea sprite si 4.pozitia sprite/hitbox
	tile1 = new Tile("IDLE.png", sf::IntRect({ 0, 0 }, { 64, 64 }), sf::IntRect({ 15, 30 }, { 64, 54 }), { 0, 50 } );

	tile1->setScale({ 2.0f, 2.0f });
	tile2 = new Tile("Soldier.png", sf::IntRect({ 0, 0 }, { 200, 200 }), sf::IntRect({ 20, 20 }, { 64, 64 }), { 200, 50 } );
	tile3 = new Tile("Soldier.png", sf::IntRect({ 0, 0 }, { 200, 200 }), sf::IntRect({ 20, 20 }, { 64, 64 }), { 200, -150 } );
	tile3->setScale({ 2.5f, 2.5f });
	tile4 = new Tile("Soldier.png", sf::IntRect({ 0, 0 }, { 200, 200 }), sf::IntRect({ 20, 20 }, { 64, 64 }), { 400, 50 } );
	
}

GameEngine::GameEngine()
{
	initWindow();
	initPlayer();
	initView();
	initTile();
}

GameEngine::~GameEngine()
{
	delete player;
} 

void GameEngine::updateView()
{
	sf::Vector2f plyayerCenter = player->getPosition();
	plyayerCenter.x += player->getGlobalBounds().size.x / 2;
	plyayerCenter.y += player->getGlobalBounds().size.y / 2;
	view.setCenter(plyayerCenter);
	window.setView(view);
}

void GameEngine::updateCollisions()
{
	if (player->getPosition().y + player->getGlobalBounds().size.y  > window.getSize().y)
	{
		player->resetVelocityY();
		player->setPosition(player->getPosition().x, window.getSize().y - player->getGlobalBounds().size.y );
		
	}
}

void GameEngine::updatePlayer()
{
	player->update();
}

void GameEngine::renderPlayer()
{
	player->render(window);
}

void GameEngine::update()
{
	//Events
	while(const std::optional<sf::Event> event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();
		
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			
		
		}
		
	}
	updatePlayer();
	//updateCollisions();
	player->getCollider()->checkCollision(*tile1->getCollider(), 0.0f, window);
	player->getCollider()->checkCollision(*tile2->getCollider(), 0.0f, window);
	player->getCollider()->checkCollision(*tile3->getCollider(), 3.0f, window);
	player->getCollider()->checkCollision(*tile4->getCollider(), 0.0f, window);

	if (tile1->getCollider() && player->getCollider()) 
	{
		
	}
	if (tile2->getCollider() && player->getCollider())
	{
		
	}
	
	updateView();
}

void GameEngine::render()
{
	window.clear();

	renderPlayer();
	tile1->render(window);
	tile2->render(window);
	tile3->render(window);
	tile4->render(window);
	//tile1->getCollider()->renderCollider(window);

	window.display();
}

const sf::RenderWindow& GameEngine::getWindow() const
{
	return this->window;
}
