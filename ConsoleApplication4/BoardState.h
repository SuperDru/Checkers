#pragma once
#include "Board.h"
#include "Eatable.h"
#include <list>
#include <vector>


class BoardState {
private:
	char** board;

	char curX, curY;

	std::vector<BoardState*>* getEatableMoves();
	std::vector<BoardState*>* getMoves();

	std::vector<BoardState*>* recursEat(int x, int y, int dx, int dy); 
	std::vector<BoardState*>* recursEatQueen(int xStart, int yStart, int enemyX, int enemyY, int x, int y);
	void dealEatable(int x, int y, int dx, int dy, std::vector<BoardState*>* result);
	void dealEatableQueen(int x, int y, int enemyX, int enemyY, int moveX, int moveY, std::vector<BoardState*>* result);


	void addMove();
public:
	bool eat;
	char movableX, movableY,
		 moveX, moveY;
	char currentColor, botColor;
	std::list<Eatable*> eaten; //if eat == true
	std::list<Eatable*> previousEaten; //if eat == true

	BoardState(int botColor);
	BoardState(Board& board);
	BoardState(BoardState& bs, std::list<Eatable*> eaten, int x, int y, int dx, int dy, int currentColor);


	std::vector<BoardState*>* getPossibleMoves();
	int getBotCount();
	int getPlayerCount();
	int getHeuristics(int bCount, int pCount);

	bool operator==(BoardState& bs);

	~BoardState();
};

