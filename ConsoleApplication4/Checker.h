#pragma once
#include <SFML\Graphics.hpp>

class Checker
{
private:
	int x, y;
	int radius;

	sf::Color color;
	sf::Texture blackChk, whiteChk, blackChkHighlited, whiteChkHighlited, 
		blackQueen, whiteQueen, blackQueenHighlited, whiteQueenHighlited;
	sf::CircleShape sprite;

	bool king;

public:
	Checker(sf::Color color, int x, int y, int radius);

	void move(int dx, int dy, int dif);

	sf::CircleShape getRenderingChecker();
	void setHighlighted();
	void resetHighlighted();
	void reset();
	void setKing();

	int getX();
	int getY();
	bool isKing();
	sf::Color getColor();

	~Checker();
};

