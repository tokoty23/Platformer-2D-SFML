#pragma once
#include "stdafx.h"
#include "GameplayState.h"

GameplayState::GameplayState(GameEngine& game, sf::Font& font) 
	: game(game)
{
	if (!buttonTexture.loadFromFile("Textures/MenuSprites/OnlyPause.png"))
	{
		std::cerr << "ERROR Class GameplayState: Failed to load button texture" << std::endl;
	}
	sf::IntRect buttonRect({ 0, 0 }, { Constants::PAUSE_SPRITE_SIZE_X, Constants::PAUSE_SPRITE_SIZE_Y });
	options.emplace_back(AnimatedSprite(buttonTexture, buttonRect));
	for (int i = 0; i < options.size(); ++i)
	{
		options[i].addAnimation("IDLE", 1, sf::IntRect({ 0, (Constants::PAUSE_SPRITE_SIZE_Y * 3 * i) }, Constants::PAUSE_SPRITE_SIZE), 0.2f, false);
		options[i].addAnimation("HOVER", 1, sf::IntRect({ 0, Constants::PAUSE_SPRITE_SIZE_Y * (3 * i + 1) }, Constants::PAUSE_SPRITE_SIZE), 0.2f, false);
		options[i].addAnimation("PRESSED", 1, sf::IntRect({ 0, Constants::PAUSE_SPRITE_SIZE_Y * (3 * i + 2) }, Constants::PAUSE_SPRITE_SIZE), 0.2f, false);

		options[i].setPosition(sf::Vector2f(game.getWindow().getSize().x  - 100,
			options[i].getGlobalBounds().size.y));
		options[i].setScale(sf::Vector2f(2.0f, 2.0f));
		options[i].changeAnimation("IDLE");
	}
	counter = 0;
	currentOption = 0;

	if (!backgroundTexture.loadFromFile("Textures/Backgrounds/MountainBackground.png"))
	//if (!backgroundTexture.loadFromFile("Textures/MenuSprites/OnlyPause.png"))
	{
		std::cerr << "ERROR Class GameplayState: Failed to load button texture" << std::endl;
	}
	background = std::make_unique<Background>(backgroundTexture, game.getWindow());
	background->setParallaxFactor(0.5f, 0.0f);

}



void GameplayState::handleInput(GameEngine& engine, std::optional<sf::Event>& event)
{
	// TODO: window lost focus -> pause the game
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
		{
			game.pushState(GameState::PAUSE_MENU);
		}
	}
	if (const auto* resized = event->getIf<sf::Event::Resized>())
	{
		std::cout << "TEST" << std::endl << std::endl << std::endl << std::endl << std::endl;
		options[0].setPosition(sf::Vector2f(game.getWindow().getSize().x - 100,
			options[0].getGlobalBounds().size.y));
		std::cout << "new width: " << resized->size.x << std::endl;
		std::cout << "new height: " << resized->size.y << std::endl;
	}

	else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
	{
		for (int i = 0; i < options.size(); ++i)
		{
			if (options[i].getGlobalBounds().contains(sf::Vector2f(mouseMoved->position.x, mouseMoved->position.y)))
			{
				if (i != currentOption)
				{
					options[currentOption].changeAnimation("IDLE");
					options[i].changeAnimation("HOVER");
					currentOption = i;
				}
				else options[currentOption].changeAnimation("HOVER");
			}
			else options[i].changeAnimation("IDLE");
		}
	}
	else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouseButtonPressed->button == sf::Mouse::Button::Left)
		{
			for (int i = 0; i < options.size(); ++i)
			{
				if (options[i].getGlobalBounds().contains(sf::Vector2f(mouseButtonPressed->position.x, mouseButtonPressed->position.y)))
				{
					if (i == currentOption)
					{
						options[i].changeAnimation("PRESSED");
					}
					else
					{
						std::cout << "else";
						options[currentOption].changeAnimation("IDLE");
						options[i].changeAnimation("PRESSED");
						currentOption = i;
					}

				}

			}
		}

	}
	else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
	{
		if (mouseButtonReleased->button == sf::Mouse::Button::Left)
		{
			for (int i = 0; i < options.size(); ++i)
			{
				if (options[i].getGlobalBounds().contains(sf::Vector2f(mouseButtonReleased->position.x, mouseButtonReleased->position.y)))
				{
					switch (i)
					{
					case 0: // Start
						game.pushState(GameState::PAUSE_MENU);
						break;
					case 1: // Exit
						game.getWindow().close();
						break;
					}
				}
				else
				{
					for (int i = 0; i < options.size(); ++i)
					{
						options[i].changeAnimation("IDLE");
					}
					options[currentOption].changeAnimation("HOVER");
				}

			}
		}

	}

}
void GameplayState::update(GameEngine& engine, sf::Time deltaTime)
{
	for (auto& option : options)
	{
		option.update(deltaTime);
	}
	game.runGameplay(deltaTime);
	if (game.getPlayer()->getState() == EntityState::E_DEAD)
	{
		game.pushState(GameState::RESPAWN_MENU);
	}
	background->updateBackground(game.getPlayer()->getPosition() );
}
void GameplayState::render(sf::RenderTarget& target)
{
	
	//game.getWindow().draw(*backgroundImage);
	background->render(target);
	game.renderGameplay();
	for (const auto& option : options)
	{
		option.render(game.getWindow());
	}
	//game.getWindow().setView(game.getWindow().getDefaultView());

}

void GameplayState::resizeWindow(sf::RenderTarget& window)
{
	background->resizeToWindow(window);
	for (int i = 0; i < options.size(); ++i)
	{
		options[i].setPosition(sf::Vector2f(game.getWindow().getSize().x - 100,
			options[i].getGlobalBounds().size.y));
		options[i].setScale(sf::Vector2f(2.0f, 2.0f));
	}
}
