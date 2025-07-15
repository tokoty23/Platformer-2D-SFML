#pragma once

class Animation
{
private:
	std::vector<sf::IntRect> frames;
	// frameTime reprezinta cat trebuie sa dureze fiecare frame in secunde, iar currenntTime cat a trecut de la ultimul frame
	float frameTime;
	float currentTime;
	int currentFrameIndex;
	bool looping;

public:
	Animation(int frameCount, sf::IntRect frameSize, float frameTime, bool looping = true);
	Animation();
	~Animation() = default;
	void playAnimation(sf::Sprite& sprite, sf::Time deltaTime, bool mirrored = false);
	void resetAnimation();
};

