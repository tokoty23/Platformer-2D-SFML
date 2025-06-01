#pragma once

enum class UserInputData
{
	U_IDLE = 0,
	U_MOVE_LEFT,
	U_MOVE_RIGHT,
	U_MOVE_UP,
	U_MOVE_DOWN,
	U_ATTACK,
	U_DASH
};

class UserInput
{
private:
	std::map<UserInputData, sf::Keyboard::Key> keybinds;

public:
	UserInput();
	~UserInput() = default;
	void changeKeybind(UserInputData action, sf::Keyboard::Key key);
	std::vector<UserInputData> getUserInput() const;
};


