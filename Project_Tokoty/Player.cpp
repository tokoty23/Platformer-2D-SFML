#include "stdafx.h"
#include "Player.h"


void Player::initVariables()
{
	animationState = Player_AnimationStates::IDLE;
	userInput = std::make_unique<UserInput>();
}

void Player::initTexture()
{
	if (!texture.loadFromFile("Textures/spritesheet.png"))
	{
		std::cout << "ERROR Class Player: initTexture()";
	}
	

}

void Player::initSprite()
{
	

	sprite.emplace(texture);
	currentFrame = sf::IntRect({ 0, 0 }, { 100, 100 }); //{top left corner} {wide tall}
	sprite->setTextureRect(currentFrame);
	sprite->setScale({ 5.0f, 5.0f });
	//std::cout << sprite->getGlobalBounds().size.x<< sprite->getGlobalBounds().size.x<<std::endl;
	//sprite->setScale({ 5.0f, 5.0f });
	//std::cout << sprite->getGlobalBounds().size.x << sprite->getGlobalBounds().size.x << std::endl;
	hitboxPlayer = std::make_unique<Collider>(currentFrame, 0.0f); // collider size is the same as sprite size
}

void Player::initAnmation()
{

}

void Player::initPhysics()
{
	velocity = sf::Vector2f(0.0f, 0.0f);
	maxVelocity = 20.0f;
	minVelocity = 1.0f;
	acceleration = 3.0f;
	friction = 0.80f;
	gravity = 3.0f;
	maxGravity = 30.0f;
}



Player::Player(std::string textureName, sf::IntRect currentRect, sf::Vector2f position)
{

	animatedSprite = std::make_unique<AnimatedSprite>(textureName, currentRect, position);
	//std::string textureName, sf::IntRect currentRect, sf::Vector2f position
	animatedSprite->addAnimation("IDLE", 4, sf::IntRect({ 0, 0 }, { 100, 100 }), 0.2f);
	animatedSprite->addAnimation("MOVE_LEFT", 4, sf::IntRect({ 0, 100 }, { 100, 100 }), 0.2f);
	animatedSprite->addAnimation("MOVE_RIGHT", 4, sf::IntRect({ 0, 200 }, { 100, 100 }), 0.2f);
	animatedSprite->addAnimation("MOVE_UP", 4, sf::IntRect({ 0, 300 }, { 100, 100 }), 0.2f);
	animatedSprite->addAnimation("MOVE_DOWN", 4, sf::IntRect({ 0, 400 }, { 100, 100 }), 0.2f);
	//std::string name, int frameCount, sf::IntRect frameSize, float frameTime, bool looping
		
	initTexture();
	initSprite();	
	initAnmation();
	initPhysics();
	initVariables();
	
}

Player::~Player()
{

}

const sf::Vector2f Player::getPosition() const
{
	return hitboxPlayer->getPosition();
}

const sf::FloatRect Player::getGlobalBounds() const
{
	return hitboxPlayer->getGlobalBounds();
}

void Player::setPosition(const float x, const float y)
{
	hitboxPlayer->setPosition(x, y);
}

void Player::resetVelocityY()
{
	velocity.y = 0.0f;
}

void Player::move(const float x, const float y)
{
	velocity.x += x * acceleration;
	velocity.y += y * acceleration;

	if (std::abs(velocity.x) > maxVelocity) velocity.x = maxVelocity * ((velocity.x < 0) ? -1.0f : 1.0f);
	//if (std::abs(velocity.y) > maxVelocity) velocity.y = maxVelocity * ((velocity.y < 0) ? -1.0f : 1.0f);
	
}

void Player::updatePhysics()
{
	//Gravity
	velocity.y += 1.0f * gravity;
	if (std::abs(velocity.y) > maxGravity) velocity.y = maxGravity * ((velocity.y < 0) ? -1.0f : 1.0f);

	velocity *= friction;

	if (std::abs(velocity.x) < minVelocity) velocity.x = 0.0f;
	if (std::abs(velocity.y) < minVelocity) velocity.y = 0.0f;

	hitboxPlayer->move(velocity);

	
	//std::cout << "velocity.x: " << velocity.x << "        ";
	//std::cout << "velocity.y: " << velocity.y << std::endl;

}

void Player::updateMovement()
{
	animationState = Player_AnimationStates::IDLE;
	userInputData = userInput->getUserInput();

	for(const auto& input : userInputData)
	{
		switch (input)
		{
			case UserInputData::U_MOVE_LEFT:
				move(-2.0f, 0.0f);
				animatedSprite->playAnimation("MOVE_LEFT", 0.05f);
				animationState = Player_AnimationStates::LEFT;
				break;

			case UserInputData::U_MOVE_RIGHT:
				move(2.0f, 0.0f);
				animatedSprite->playAnimation("MOVE_RIGHT", 0.05f);
				animationState = Player_AnimationStates::RIGHT;
				break;

			case UserInputData::U_MOVE_UP:
				move(0.0f, -2.0f);
				animatedSprite->playAnimation("MOVE_UP", 0.05f);
				animationState = Player_AnimationStates::JUMP;
				break;

			case UserInputData::U_MOVE_DOWN:
				move(0.0f, 2.0f);
				animatedSprite->playAnimation("MOVE_DOWN", 0.05f);
				animationState = Player_AnimationStates::FALL;
				break;

			default:
				break;
		}	
	}
}

void Player::updateAnimation()
{
	if (animationState == Player_AnimationStates::IDLE)
	{
		if (animationClock.getElapsedTime().asSeconds() >= 0.1f)
		{
			currentFrame.position.x += 100;
			currentFrame.position.y = 0;
			if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
			animationClock.restart();
			sprite->setTextureRect(currentFrame);
		}
	}
	
	else 
	{
		if (animationClock.getElapsedTime().asSeconds() >= 0.1f)
		{

			{
				switch (animationState)
				{
				case Player_AnimationStates::LEFT:
					currentFrame.position.x += 100;
					currentFrame.position.y = 200;
					if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
					if (currentFrame.size.x > 0) currentFrame.size.x *= -1;


					break;

				case Player_AnimationStates::RIGHT:
					currentFrame.position.x += 100;
					currentFrame.position.y = 200;
					if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
					if (currentFrame.size.x < 0) currentFrame.size.x *= -1;
					//sprite->setScale({ 5.0f, 5.0f });
					//sprite->setOrigin({ 0, 0 });
					break;

				case Player_AnimationStates::JUMP:
					currentFrame.position.x += 100;
					currentFrame.position.y = 300;
					if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
					break;

				case Player_AnimationStates::FALL:
					currentFrame.position.x += 100;
					currentFrame.position.y = 400;
					if (currentFrame.position.x > 100 * 5) currentFrame.position.x = 0;
					break;

				default:

					break;
				}
			}
			animationClock.restart();
			sprite->setTextureRect(currentFrame);
		}
	}
}

void Player::update()
{
	updateMovement();
	updateAnimation();
	updatePhysics();
}

void Player::render(sf::RenderTarget& target)
{
	if (sprite)
	{
		sprite->setPosition(hitboxPlayer->getPosition());
		animatedSprite->setPosition(hitboxPlayer->getPosition());

		/*
		std::cout << "Sprite pos: " << sprite->getGlobalBounds().size.x << ", " << sprite->getGlobalBounds().size.y << std::endl;
		std::cout << "Collider pos: " << hitboxPlayer->getGlobalBounds().position.x << ", " << hitboxPlayer->getGlobalBounds().position.y << std::endl;
		std::cout << "Collider pos: " << hitboxPlayer->getPosition().x << ", " << hitboxPlayer->getPosition().y << std::endl;
		std::cout << "Collider size: " << hitboxPlayer->getGlobalBounds().size.x << ", " << hitboxPlayer->getGlobalBounds().size.y << std::endl;
		std::cout << "Collider size: " << hitboxPlayer->getSize().x << ", " << hitboxPlayer->getSize().y << std::endl;

		*/
		//std::cout << "Sprite x: " << sprite->getPosition().x << " Sprite y: " << sprite->getPosition().y << std::endl;
		// 
		//target.draw(*sprite);
		animatedSprite->render(target);
	}
	hitboxPlayer->renderCollider(target);
	sf::RectangleShape testRect(sf::Vector2f(sprite->getGlobalBounds().size.x, sprite->getGlobalBounds().size.y));
	testRect.setPosition(sf::Vector2f(sprite->getGlobalBounds().position.x, sprite->getGlobalBounds().position.y) );
	testRect.setFillColor(sf::Color(255, 0, 0, 128));
	target.draw(testRect);

	sf::RectangleShape circle(sf::Vector2f(sprite->getGlobalBounds().size.x, sprite->getGlobalBounds().size.y));
	//circle.setRadius(50.0f);
	circle.setFillColor(sf::Color(255, 0, 0, 128)); // Red, half transparent
	circle.setPosition(sprite->getPosition());
	//target.draw(circle);
	
	
}
