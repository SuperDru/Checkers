#pragma once
#include "BotTree.h"

class Bot{
private:
	BotTree* botTree = nullptr;
	std::string color;

	bool eat;
	sf::Vector2i movableCoord;
	sf::Vector2i eatableCoord; //if eat == true
	sf::Vector2i movePoint;
public:
	Bot(std::string color);

	void makeMove(Board& board);

	~Bot();
};

