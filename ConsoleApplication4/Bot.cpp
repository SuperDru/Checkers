#include "Bot.h"
#include <iostream>
#include <Windows.h>


Bot::Bot(std::string color) {
	this->color = color;
}

void Bot::makeMove(Board& board)     {
	if (botTree == nullptr) {
		BoardState* state = new BoardState(board);
		botTree = new BotTree(*state, 0);
	}
	else
		botTree->next(board);

	int result = botTree->max();

	if (result == 5000)
		//std::cout << "1\n";
		MessageBoxA(NULL, "You have lost!", "Loser", MB_OK | MB_ICONEXCLAMATION);
	else if (result == -5000 && botTree->resultTree == nullptr || botTree->resultTree == nullptr)
		MessageBoxA(NULL, "You have won!", "Winner!", MB_OK | MB_ICONEXCLAMATION);
	else {
		BoardState* bs = &botTree->getResultState();

		if (bs->eat)
			board.eatChk(bs->movableX, bs->movableY, bs->previousEaten);
		else
			board.makeMove(bs->movableX, bs->movableY, bs->moveX, bs->moveY);
	}
}


Bot::~Bot(){}
