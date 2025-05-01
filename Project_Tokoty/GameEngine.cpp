#include "stdafx.h"
#include "GameEngine.h"

void GameEngine::initWindow()
{
	window.create(sf::VideoMode({800,600}), "Project Tokoty", sf::Style::Close | sf::Style::Titlebar); //VideoMode::getDesktopMode()
	window.setFramerateLimit(144); // customize option to be added
}

void GameEngine::initPlayer()
{
	player = new Player();
}

GameEngine::GameEngine()
{
	initWindow();
	initPlayer();
}

GameEngine::~GameEngine()
{
	delete player;
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
}

void GameEngine::render()
{
	window.clear();

	renderPlayer();

	window.display();
}

const sf::RenderWindow& GameEngine::getWindow() const
{
	return this->window;
}
