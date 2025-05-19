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
	player = new Player();
}

void GameEngine::initTile()
{
	tile1 = new Tile();
	
}

GameEngine::GameEngine()
{
	initWindow();
	initPlayer();
	//initView();
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
	updateCollisions();
	std::cout << tile1->getCollider()->getPosition().x;
	if (tile1->getCollider() && player->getCollider()) {
		
		tile1->getCollider()->checkCollision(*player->getCollider(), 0.0f);
	}
	//updateView();
}

void GameEngine::render()
{
	window.clear();

	renderPlayer();
	tile1->render(window);


	window.display();
}

const sf::RenderWindow& GameEngine::getWindow() const
{
	return this->window;
}
