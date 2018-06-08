#pragma once
#include "Board.h"
#include "BotTree.h"

class GameDispatcher
{
private:
	sf::RenderWindow window;
	int width, height;
public:
	GameDispatcher(int width, int height, int x, int y);
	
	void launchMapLoading();

	~GameDispatcher();
};

