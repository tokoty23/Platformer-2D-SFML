#include "stdafx.h"
#include "HealthBar.h"

HealthBar::HealthBar(Player* player)
	: player(player)
{
    if (!healthBarTexture.loadFromFile("Textures/Hearts_spritesheet.png"))
    {
        std::cerr << "ERROR Class HealthBar: constructor()" << std::endl;
    }

    position = sf::Vector2f(10.0f, 10.0f); 
    offset = 5.0f;

    heartRect = sf::IntRect({ 0, 0 }, { Constants::HEALRT_SIZE, Constants::HEALRT_SIZE });
    emptyHeartRect = sf::IntRect({ Constants::HEALRT_SIZE, 0 }, { Constants::HEALRT_SIZE, Constants::HEALRT_SIZE });
    
	int maxHealth = player->getMaxHealth();
	int heartCount = std::ceil(maxHealth / Constants::ONE_HEART_VALUE);

    for (int i = 0; i < heartCount; ++i)
    {
        StaticSprite heartSprite(healthBarTexture, heartRect);
        heartSprite.setPosition({ position.x + i * (Constants::HEALRT_SIZE + offset), position.y });
        hearts.push_back(heartSprite);
	}
}

void HealthBar::update(sf::Time deltaTime)
{
	if (!player) return; 

	int currentHealth = player->getHealth();
	int fullHearts = currentHealth / Constants::ONE_HEART_VALUE;

    for (auto i = 0; i < hearts.size(); i++)
    {
        if (i < fullHearts)
        {
            hearts[i].setTextureRect(heartRect);
        }
        else
        {
            hearts[i].setTextureRect(emptyHeartRect);
		}
    }
}

void HealthBar::render(sf::RenderTarget& target)
{
    for (const auto& heart : hearts)
    {
		heart.render(target);
	}
}