#include "stdafx.h"
#include "MainMenuState.h"


MainMenuState::MainMenuState(GameEngine& game, sf::Font& font)
	: game(game),font(font), title(font)
{
	if (!buttonTexture.loadFromFile("Textures/MenuSprites/MainMenuButtons.png"))
	{
		std::cerr << "ERROR Class MainMenuState: Failed to load button texture" << std::endl;
	}
	title.setCharacterSize(50);
	title.setFillColor(sf::Color::Blue);
	title.setOutlineColor(sf::Color::Black);
	title.setString("Project Tokoty");
	title.setPosition(sf::Vector2f(game.getWindow().getSize().x / 2 - title.getGlobalBounds().size.x / 2, 50));
	sf::IntRect buttonRect({ 0, 0 }, { Constants::BUTTON_SPRITE_SIZE_X, Constants::BUTTON_SPRITE_SIZE_Y });
	sf::IntRect buttonRect2({ 0,  3 * Constants::BUTTON_SPRITE_SIZE_Y }, { Constants::BUTTON_SPRITE_SIZE_X, Constants::BUTTON_SPRITE_SIZE_Y });
	options.emplace_back(AnimatedSprite(buttonTexture, buttonRect));
	options.emplace_back(AnimatedSprite(buttonTexture, buttonRect2));
	for (int i = 0; i < options.size(); ++i)
	{
		options[i].addAnimation("IDLE", 1, sf::IntRect({ 0, (Constants::BUTTON_SPRITE_SIZE_Y * 3 * i) }, Constants::BUTTON_SPRITE_SIZE), 0.2f, false);
		options[i].addAnimation("HOVER", 2, sf::IntRect({ 0, Constants::BUTTON_SPRITE_SIZE_Y * (3 * i + 1)  }, Constants::BUTTON_SPRITE_SIZE), 0.5f);
		options[i].addAnimation("PRESSED", 1, sf::IntRect({ 0, Constants::BUTTON_SPRITE_SIZE_Y * (3 * i + 2) }, Constants::BUTTON_SPRITE_SIZE), 0.2f, false);

		options[i].setPosition(sf::Vector2f(game.getWindow().getSize().x / 2 - options[i].getGlobalBounds().size.x,
			game.getWindow().getSize().y /4 + i * 150));
		options[i].setScale(sf::Vector2f(2.0f, 2.0f));
		options[i].changeAnimation("IDLE");
	}
	currentOption = 0;
	options[currentOption].changeAnimation("HOVER");
}

void MainMenuState::handleInput(GameEngine& engine, std::optional<sf::Event>& event)
{
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) 
			{
				//game.getWindow().close();
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::Up ||
				keyPressed->scancode == sf::Keyboard::Scancode::W)
			{
				options[currentOption].changeAnimation("IDLE");
				currentOption = (currentOption - 1 + options.size()) % options.size();
				options[currentOption].changeAnimation("HOVER");
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::Down ||
				keyPressed->scancode == sf::Keyboard::Scancode::S)
			{
				options[currentOption].changeAnimation("IDLE");
				currentOption = (currentOption + 1) % options.size();
				options[currentOption].changeAnimation("HOVER");
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter ||
				keyPressed->scancode == sf::Keyboard::Scancode::Space)
			{
				switch (currentOption)
				{
				case 0: // Start
					game.pushState(GameState::GAMEPLAY);
					break;
				case 1: // Exit
					game.getWindow().close();
					break;
				}
			}
		}

		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			for ( int i = 0; i < options.size(); ++i)
			{
				
				if (options[i].getGlobalBounds().contains(sf::Vector2f(mouseMoved->position.x, mouseMoved->position.y)))
				{
					if (i != currentOption)
					{
						options[currentOption].changeAnimation("IDLE");
						options[i].changeAnimation("HOVER");
						currentOption = i;
					}	
				}
			}
			//std::cout << "Mouse position: " << mouseMoved->position.x << ", " << mouseMoved->position.y << std::endl;
			//std::cout << "Text position: " << options[0].getGlobalBounds().position.x << ", " << options[0].getGlobalBounds().position.y << std::endl;
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
							std::cout << "false" << std::endl;
						}
						else
						{
							options[currentOption].changeAnimation("IDLE");
							std::cout << "true" << std::endl;
							options[i].changeAnimation("PRESSED");
							currentOption = i;
						}

					}

				}
				//std::cout << "Mouse position: " << mouseButtonPressed->position.x << ", " << mouseButtonPressed->position.y << std::endl;
				//std::cout << "Text position: " << options[0].getGlobalBounds().position.x << ", " << options[0].getGlobalBounds().position.y << std::endl;
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
							game.pushState(GameState::GAMEPLAY);
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

void MainMenuState::update(GameEngine& engine, sf::Time deltaTime)
{
	for (auto& option : options)
	{
		option.update(deltaTime);
	}
}

void MainMenuState::render(sf::RenderTarget& target)
{
	//game.getWindow().setView(game.getWindow().getDefaultView());
	game.getWindow().draw(title);
	for (const auto& option : options)
	{
		option.render(game.getWindow());
	}
}

void MainMenuState::resizeWindow(sf::RenderTarget& window)
{

}
