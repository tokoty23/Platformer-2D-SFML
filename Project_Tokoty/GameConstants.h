#pragma once

namespace Constants
{
	//Healthbar
	constexpr int HEALRT_SIZE = 48;
	constexpr int ONE_HEART_VALUE = 20; // o inima reprezinta [ONE_HEART_VALUE] hp

	//MainMenuState
	constexpr int BUTTON_SPRITE_SIZE_Y = 48;
	constexpr int BUTTON_SPRITE_SIZE_X = 144;
	constexpr auto BUTTON_SPRITE_SIZE = sf::Vector2i(BUTTON_SPRITE_SIZE_X, BUTTON_SPRITE_SIZE_Y);

	//PauseMenuState
	constexpr int PAUSE_SPRITE_SIZE_X = 48;
	constexpr int PAUSE_SPRITE_SIZE_Y = 48;
	constexpr auto PAUSE_SPRITE_SIZE = sf::Vector2i(PAUSE_SPRITE_SIZE_X, PAUSE_SPRITE_SIZE_Y);
}
