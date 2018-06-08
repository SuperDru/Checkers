#pragma once
#include "BoardState.h"



class BotTree {
private:
	static int bCount, pCount;
	const int HEIGHT = 5;
	std::list<BotTree*> childs;
	BoardState* state;

	void findBoard(Board& board);
	void setChilds();
public:

	BotTree* resultTree = nullptr;

	BotTree(BoardState& stardState, int height);

	int min(float alpha = -10000., float beta = 10000.);
	int max(float alpha = -10000., float beta = 10000.);

	BoardState& getResultState();

	void next(Board& board);

	bool isTerminal();

	BotTree& operator=(const BotTree& bt);

	~BotTree();
};

