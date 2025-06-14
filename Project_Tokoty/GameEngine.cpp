﻿#include "stdafx.h"
#include "GameEngine.h"

void GameEngine::initView()
{
	sf::Vector2f plyayerCenter = player->getPosition();
	plyayerCenter.x += player->getGlobalBounds().size.x / 2;
	plyayerCenter.y += player->getGlobalBounds().size.y / 2 + window.getSize().y / static_cast<float>(4);
	//view.setCenter(plyayerCenter);
	//window.setView(view);
}

void GameEngine::initWindow()
{
	window.create(sf::VideoMode({800,600}), "Project Tokoty", sf::Style::Close | sf::Style::Titlebar); //VideoMode::getDesktopMode()
	//window.create(sf::VideoMode::getDesktopMode(), "Project Tokoty", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60); // customize option to be added
}

void GameEngine::initPlayer()
{
	std::string textureName = "SPRITESHEET_1.png";
	sf::IntRect sizeSprite = sf::IntRect({ 0, 0 }, { 48, 48 }); // Primul cadru pentru sprite
	sf::IntRect sizeHitbox = sf::IntRect({ 0, 0 }, { 48-28, 48-12 }); // Dimensiunea pentru hitbox
	sf::Vector2f startPosition = { 200.f, -300.f };

	auto animatedSpriteComponent = std::make_unique<AnimatedSprite>(textureName, sizeSprite, startPosition);
	auto colliderComponent = std::make_unique<Collider>(
		sf::FloatRect({ startPosition }, { float(sizeHitbox.size.x), float(sizeHitbox.size.y) }),
		startPosition,
		0.0f
	);

	player = std::make_unique<Player>(
		std::move(colliderComponent),
		std::move(animatedSpriteComponent)
	);

	player->setScale({ 5.0f, 5.0f });

	sf::Vector2f startPosition2 = { 400.f, -300.f };
	auto animatedSpriteComponent2 = std::make_unique<AnimatedSprite>(textureName, sizeSprite, startPosition);
	auto colliderComponent2 = std::make_unique<Collider>(
		sf::FloatRect({ startPosition2 }, { float(sizeHitbox.size.x), float(sizeHitbox.size.y) }),
		startPosition,
		0.0f, ColliderType::C_HITBOX_HURTBOX
	);
	
	enemy = std::make_unique<Enemy>(
		std::move(colliderComponent2),
		std::move(animatedSpriteComponent2)
	);
	enemy->setScale({ 2.0f, 2.0f });
}

void GameEngine::initTile()
{


	tileMap = new TileMap("map3test.tmx", "SlimeGreen1.png", sf::Vector2f(2.0f, 2.0f) );
	// tiles are ca parametri 1.imaginea 2.intrect pentru marimea hitbox, 3.pentru marimea sprite si 4.pozitia sprite/hitbox
	//tile1 = new Tile("IDLE.png", sf::IntRect({ 0, 0 }, { 64, 64 }), sf::IntRect({ 15, 30 }, { 64, 54 }), { 0, 50 } );

	tile2 = new Tile("Soldier.png", sf::IntRect({ 0, 0 }, { 200, 200 }), sf::IntRect({ 20, 20 }, { 64, 64 }), { 200, 50 });
	tile3 = new Tile("Soldier.png", sf::IntRect({ 0, 0 }, { 200, 200 }), sf::IntRect({ 20, 20 }, { 64, 64 }), { 200, -150 }, false, true);
	tile3->setScale({ 2.5f, 2.5f });
	tile4 = new Tile("Soldier.png", sf::IntRect({ 0, 0 }, { 200, 200 }), sf::IntRect({ 20, 20 }, { 64, 64 }), { 400, 50 });
	
}

GameEngine::GameEngine()
{
	initTile();
	initWindow();
	initPlayer();
	initView();
	
}

GameEngine::~GameEngine()
{
	//delete player;
} 

void GameEngine::updateView()
{
	sf::Vector2f plyayerCenter = player->getPosition();
	plyayerCenter.x += player->getGlobalBounds().size.x / 2;
	plyayerCenter.y += player->getGlobalBounds().size.y / 2 - window.getSize().y / static_cast<float>(4);
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

void GameEngine::updatePlayer(float deltaTime)
{
	player->update(deltaTime);
	enemy->update(deltaTime);
}

void GameEngine::renderPlayer()
{
	player->render(window);
	enemy->render(window);
}

void GameEngine::update()
{
	float deltaTime = deltaClock.restart().asSeconds();
	
	//Events
	while(const std::optional<sf::Event> event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();
		
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			
		
		}
		
	}
	updatePlayer(deltaTime);
	//updateCollisions();
	//player->getCollider()->checkCollision(*tile1->getCollider(), 0.0f);
	player->getCollider()->checkCollision(*tile2->getCollider(), 0.0f);
	player->getCollider()->checkCollision(*tile3->getCollider(), 3.0f);
	player->getCollider()->checkCollision(*tile4->getCollider(), 0.0f);
	player->getCollider()->checkCollision(*enemy->getCollider(), 0.5f);

	enemy->getCollider()->checkCollision(*tile2->getCollider(), 0.0f);
	enemy->getCollider()->checkCollision(*tile3->getCollider(), 0.0f);
	enemy->getCollider()->checkCollision(*tile4->getCollider(), 0.0f);
	tileMap->update(player.get());
	tileMap->update(enemy.get());
	updateView();
}

void GameEngine::render()
{
	window.clear();

	renderPlayer();
	//tile1->render(window);
	tile2->render(window);
	tile3->render(window);
	tile4->render(window);
	tileMap->render(window);

	window.display();
}

const sf::RenderWindow& GameEngine::getWindow() const
{
	return this->window;
}
