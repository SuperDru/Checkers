#include "bottree.h"

int BotTree::bCount;
int BotTree::pCount;

BotTree::BotTree(BoardState& startState, int height){
	state = &startState;

	if (height != HEIGHT) {
		std::vector<BoardState*>* vec = startState.getPossibleMoves();
		for (int i = 0; i < vec->size(); i++) 
			childs.push_back(new BotTree(*vec->at(i), height + 1));
	}
}

int BotTree::min(float alpha, float beta){
	if (isTerminal())
		return state->getHeuristics(bCount, pCount);

	int result = beta, temp;
	for (auto child : childs) {
		temp = child->max(alpha, result);
		if (temp < result) {
			result = temp;
			resultTree = child;
		}
		if (result < alpha) return result;
	}
	return result;
}
int BotTree::max(float alpha, float beta){
	if (isTerminal())
		return state->getHeuristics(bCount, pCount);

	if (alpha == -10000 && beta == 10000) {
		bCount = state->getBotCount();
		pCount = state->getPlayerCount();
	}

	int result = alpha, temp;
	for (auto child : childs) {
		temp = child->min(result, beta);
		if (temp > result) {
			result = temp;
			resultTree = child;
		}
		if (result > beta) return result;
	}
	return result;
}

void BotTree::next(Board& board) {
	for (auto && tr: childs) {
		if (tr != resultTree)
			delete tr;
		else
			tr->findBoard(board);
	}
	*this = *resultTree;
	setChilds();
}
void BotTree::findBoard(Board& board) {
	BoardState bs(board);
	BotTree *temp = nullptr;
	for (auto && tr : childs) {
		if (!(*tr->state == bs))
			delete tr;
		else
			temp = tr;
	}
	if (temp != nullptr)
		*this = *temp;
}
void BotTree::setChilds() {
	if (!childs.empty()) {
		for (auto && child : childs)
			child->setChilds();
	}
	else {
		std::vector<BoardState*>* vec = state->getPossibleMoves();
		for (int i = 0; i < vec->size(); i++)
			childs.push_back(new BotTree(*vec->at(i), HEIGHT - 1));
	}
}

BoardState& BotTree::getResultState() {
	return *resultTree->state;
}

bool BotTree::isTerminal() {
	return childs.empty();
}


BotTree& BotTree::operator=(const BotTree& bt) {
	childs = bt.childs;
	state = bt.state;
	resultTree = bt.resultTree;
	return *this;
}

BotTree::~BotTree(){
	for (auto && tr: childs) {
		delete tr;
	}
	delete state;
}
