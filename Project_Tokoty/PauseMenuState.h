#pragma once
#include "State.h"

class PauseMenuState : public State
{
private:
	sf::Texture buttonTexture;
	GameEngine& game;
	sf::Text title;
	std::vector<AnimatedSprite> options;
	int currentOption;
	sf::Font& font;
public:
	PauseMenuState(GameEngine& game, sf::Font& font);
	void handleInput(GameEngine& engine, std::optional<sf::Event>& event) override;
	void update(GameEngine& engine, sf::Time deltaTime) override;
	void render(sf::RenderTarget& target) override;
	void resizeWindow(sf::RenderTarget& window) override;
};

