#include "stdafx.h"
#include "Player.h"

class GameEngine
{
private:
	sf::RenderWindow window;
	//sf::Event event;
	Player* player;
	void initWindow();
	void initPlayer();
public:
	GameEngine();
	virtual ~GameEngine();

	//functions
	void updateCollisions();
	void updatePlayer();
	void renderPlayer();
	void update();
	void render();
	const sf::RenderWindow& getWindow() const;
};



