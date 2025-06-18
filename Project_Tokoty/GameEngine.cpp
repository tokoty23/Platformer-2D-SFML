#include "stdafx.h"
#include "GameEngine.h"

GameEngine::GameEngine()
{

	initWindow();
	initEntities();
	initView();
}


GameEngine::~GameEngine()
{
	//delete player;
}

void GameEngine::initWindow()
{
	window.create(sf::VideoMode({ 800,600 }), "Project Tokoty", sf::Style::Close | sf::Style::Titlebar); //VideoMode::getDesktopMode()
	//window.create(sf::VideoMode::getDesktopMode(), "Project Tokoty", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60); // customize option to be added
}

void GameEngine::initView()
{
	sf::Vector2f plyayerCenter = player->getPosition();
	plyayerCenter.x += player->getGlobalBounds().size.x / 2;
	plyayerCenter.y += player->getGlobalBounds().size.y / 2 + window.getSize().y / static_cast<float>(4);
	//view.setCenter(plyayerCenter);
	//window.setView(view);
}

void GameEngine::initEntities()
{
	tileMap = std::make_unique<TileMap>("map3test.tmx", "SlimeGreen1.png", sf::Vector2f(2.0f, 2.0f));
	
	std::string textureName = "SPRITESHEET_1_mod-sheet.png";
	sf::IntRect sizeSprite = sf::IntRect({ 0, 0 }, { 48, 48 }); // Primul cadru pentru sprite
	sf::IntRect sizeHitbox = sf::IntRect({ 200, -300 }, { 48-28, 48-12 }); // Dimensiunea pentru hitbox

	player = std::make_unique<Player>(
		std::make_unique<Collider>(sizeHitbox, 0.5),
		std::make_unique<AnimatedSprite>(textureName, sizeSprite, sf::Vector2f{ (float)sizeHitbox.position.x, (float)sizeHitbox.position.y })
	);
	player->setScale({ 5.0f, 5.0f });

	sf::IntRect enemySizeHitbox = sf::IntRect({ 400, -300 }, { 48 - 28, 48 - 12 });

	auto enemy1 = std::make_unique<Enemy>(
		std::make_unique<Collider>(enemySizeHitbox, 0.5),
		std::make_unique<AnimatedSprite>(textureName, sizeSprite, sf::Vector2f{ (float)enemySizeHitbox.position.x, (float)enemySizeHitbox.position.y })
	);
	enemy1->setScale({ 2.0f, 2.0f });
	enemies.push_back(std::move(enemy1));
}

void GameEngine::run()
{

	while (window.isOpen())
	{
		// .restart() returneaza timpul de la ultimul frame inainte de a-l reseta
		sf::Time deltaTime = deltaClock.restart();
		pollEvents();
		updateEntities(deltaTime);
		updatePhysicsColliders();
		player->syncCollidersWithHitbox();
		for (auto& enemy : enemies)
		{
			enemy->syncCollidersWithHitbox();
		}
		updateCombatColliders();
		updateView();

		//update();
		render();
	}
}

void GameEngine::pollEvents()
{
	while (const std::optional<sf::Event> event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {

		}
	}
}

void GameEngine::updateEntities(sf::Time deltaTime)
{
	player->update(deltaTime);
	for (auto& enemy : enemies)
	{
		enemy->update(deltaTime);
	}

}

void GameEngine::updatePhysicsColliders()
{
	tileMap->update(player.get());
	for (auto& enemy : enemies)
	{
		tileMap->update(enemy.get());
	}
	Collider* playerHitbox = player->getCollider(ColliderKeys::E_HITBOX);
	if (playerHitbox)
	{
		for (auto& enemy : enemies)
		{
			Collider* enemyHitbox = enemy->getCollider(ColliderKeys::E_HITBOX);
			if (enemyHitbox)
			{
				playerHitbox->checkCollision(*enemyHitbox, 0.5f);
			}
		}
	}
}

void GameEngine::updateCombatColliders()
{
	Collider* playerAttackbox = player->getCollider(ColliderKeys::E_ATTACKBOX_DEFAULT);
	if (playerAttackbox && playerAttackbox->isActive())
	{
		for (auto& enemy : enemies)
		{
			Collider* enemyHurtbox = enemy->getCollider(ColliderKeys::E_HURTBOX);
			if (enemyHurtbox && playerAttackbox->intersects(*enemyHurtbox))
			{
				playerAttackbox->setActive(false);
				enemy->takeDamage(player->getAttackDamage());
			}
		}
	}

	Collider* playerHurtbox = player->getCollider(ColliderKeys::E_HURTBOX);
	if(playerHurtbox && playerHurtbox->isActive())
	{
		for (auto& enemy : enemies)
		{
			Collider* enemyAttackbox = enemy->getCollider(ColliderKeys::E_ATTACKBOX_DEFAULT);
			if (enemyAttackbox && enemyAttackbox->isActive() && playerHurtbox->intersects(*enemyAttackbox))
			{
				enemyAttackbox->setActive(false);
				player->takeDamage(enemy->getAttackDamage());
			}
		}
	}
}

void GameEngine::updateView()
{
	sf::Vector2f plyayerCenter = player->getPosition();
	plyayerCenter.x += player->getGlobalBounds().size.x / 2;
	plyayerCenter.y += player->getGlobalBounds().size.y / 2 - window.getSize().y / static_cast<float>(4);
	view.setCenter(plyayerCenter);
	window.setView(view);
}

void GameEngine::renderEntities()
{
	player->render(window);
	for (const auto& enemy : enemies)
	{
		enemy->render(window);
	}
}

void GameEngine::renderTileMap()
{
	tileMap->render(window);
}

void GameEngine::render()
{
	window.clear();
	renderTileMap();
	renderEntities();
	window.display();
}

const sf::RenderWindow& GameEngine::getWindow() const
{
	return this->window;
}
