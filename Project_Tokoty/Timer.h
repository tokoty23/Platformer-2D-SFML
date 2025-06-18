#pragma once

class Timer
{
private:
	sf::Time remainingTime;
	sf::Time duration;
	bool active;
	
public:
	Timer();
	~Timer() {}

	void setDuration(sf::Time duration);
	void start();
	void stop();
	void pause();
	void resume();
	void update(sf::Time deltaTime);

	bool isActive() const;

};

