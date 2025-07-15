#pragma once
#include "State.h"
#include "Background.h"

class GameplayState : public State
{
private:
	sf::Texture backgroundTexture;
	std::optional<sf::Sprite> backgroundImage;
	std::unique_ptr<Background> background;

	int counter;
	GameEngine& game;
	sf::Texture buttonTexture;
	
	std::vector<AnimatedSprite> options;
	int currentOption;
public:
	GameplayState(GameEngine& game, sf::Font& font);
	void handleInput(GameEngine& engine, std::optional<sf::Event>& event) override;
	void update(GameEngine& engine, sf::Time deltaTime) override;
	void render(sf::RenderTarget& target) override;
	void resizeWindow(sf::RenderTarget& window) override;
};

