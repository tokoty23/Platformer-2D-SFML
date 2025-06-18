#include "stdafx.h"
#include "Timer.h"

Timer::Timer() 
	: remainingTime(sf::Time::Zero), duration(sf::Time::Zero), active(false)
{

}

void Timer::setDuration(sf::Time duration)
{
	this->duration = duration;
}

void Timer::start()
{
	active = true;
	remainingTime = duration;
}

void Timer::stop()
{
	active = false;
	remainingTime = sf::Time::Zero;
}

void Timer::pause()
{
	active = false;
}

void Timer::resume()
{
	if (remainingTime > sf::Time::Zero) active = true;
}

void Timer::update(sf::Time deltaTime)
{
	if (!active) return;

	remainingTime -= deltaTime;
	if (remainingTime <= sf::Time::Zero) {
		active = false;
		remainingTime = sf::Time::Zero;
	}
}

bool Timer::isActive() const
{
	return active;
}