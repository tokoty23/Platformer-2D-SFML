#include "stdafx.h"
#include "Animation.h"



Animation::Animation(int frameCount, sf::IntRect frameSize, float frameTime, bool looping)
{
	this->frameTime = frameTime;
	this->currentTime = 0.0f;
	this->currentFrameIndex = 0;
	this->looping = looping;

	for(int i = 0; i < frameCount; ++i)
	{
		sf::IntRect frameRect = frameSize;
		frameRect.position.x += i * frameSize.size.x;
		//frameRect.position.y += i * frameSize.size.y;
		//std::cout << "Playing animation frame: " << frameRect.position.x << "  "<< frameRect.position.y << std::endl;
		frames.push_back(frameRect);
	}
	for (size_t i = 0; i < frames.size(); ++i) {
		std::cout << "Frame " << i << ": "
			<< "left=" << frames[i].position.x << ", "
			<< "top=" << frames[i].position.y << ", "
			<< "width=" << frames[i].size.x << ", "
			<< "height=" << frames[i].size.x << std::endl;
	}
}

Animation::Animation()
{
	currentFrameIndex = 0;
	frameTime = 0.0f;
	currentTime = 0.0f;
	looping = false;
}


void Animation::playAnimation(sf::Sprite& sprite, sf::Time deltaTime, bool mirrored)
{
	
	currentTime += deltaTime.asSeconds();
	if (currentTime >= frameTime)
	{
		currentTime = currentTime - frameTime;
		sf::IntRect frameRect = frames[currentFrameIndex];
		
		if (mirrored)
		{
			frameRect.position.x += frameRect.size.x;
			frameRect.size.x = -frameRect.size.x;
		}
		sprite.setTextureRect(frameRect);
		//std::cout << "Playing animation frame: " << currentFrameIndex << std::endl;
		//std::cout << frames[currentFrameIndex].position.x << " " << frames[currentFrameIndex].position.y << " + " <<  std::endl;
		currentFrameIndex++;
		if (currentFrameIndex >= frames.size())
		{
			if (looping)
			{
				currentFrameIndex = 0;
			}
			else
			{
				currentFrameIndex--;
			}
		}
	}
}


