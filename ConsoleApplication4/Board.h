#pragma once
#include "Checker.h"
#include "Eatable.h"
#include <list>
#include <vector>

class Board
{
protected:

	std::vector<Checker*> checkers; 
private:
	sf::Color who; 

	bool eat = false; 
	sf::Texture blackSpr, whiteSpr;
	sf::Sprite spr;
	sf::RenderTexture sprite; 
	Checker* movableChk; 

	unsigned width, height; 

	bool isNeedEat();
	bool isNeedEat(int x, int y);
	bool isNeedEatKing(Checker* chk);

	bool move(int x, int y);

	bool tryEat(sf::Vector2i pos);

	void render();
	void renderBoard();
	void renderCheckers();

	void resetMovable();
	void setMovable(int x, int y);
public:
	Board(unsigned width, unsigned height);

	sf::Sprite getRenderingBoard();
	bool makeAction(sf::Vector2i pos);
	bool makeMove(int x, int y, int moveX, int moveY);
	bool eatChk(int x, int y, std::list<Eatable*> eaten);


	int board[8][8] = { { 0,2,0,2,0,2,0,2 },
						{ 2,0,2,0,2,0,2,0 },
						{ 0,2,0,2,0,2,0,2 },
						{ 1,0,1,0,1,0,1,0 },
						{ 0,1,0,1,0,1,0,1 },
						{ 3,0,3,0,3,0,3,0 },
						{ 0,3,0,3,0,3,0,3 },
						{ 3,0,3,0,3,0,3,0 } };

	//int board[8][8] = { { 0,5,0,1,0,1,0,1 },
	//					{ 1,0,3,0,1,0,1,0 },
	//					{ 0,2,0,2,0,1,0,1 },
	//					{ 2,0,2,0,2,0,1,0 },
	//					{ 0,3,0,2,0,1,0,1 },
	//					{ 3,0,3,0,3,0,0,0 },
	//					{ 0,3,0,1,0,3,0,1 },
	//					{ 3,0,1,0,1,0,3,0 } };
	~Board();
};

