#include "stdafx.h"
#include "UserInput.h"

UserInput::UserInput()
{
	changeKeybind(UserInputData::U_MOVE_LEFT, sf::Keyboard::Key::A);
	changeKeybind(UserInputData::U_MOVE_RIGHT, sf::Keyboard::Key::D);
	changeKeybind(UserInputData::U_MOVE_UP, sf::Keyboard::Key::W);
	changeKeybind(UserInputData::U_MOVE_DOWN, sf::Keyboard::Key::S);
}

void UserInput::changeKeybind(UserInputData action, sf::Keyboard::Key key)
{
	keybinds[action] = key;
}

std::vector<UserInputData> UserInput::getUserInput() const
{
	std::vector<UserInputData> inputs;
	for (const auto& [action, key] : keybinds)
	{
		if (sf::Keyboard::isKeyPressed(key))
		{
			inputs.push_back(action);
		}
	}
	return inputs;
}

