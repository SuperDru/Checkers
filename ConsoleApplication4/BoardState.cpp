#include "BoardState.h"


BoardState::BoardState(int botColor) {
	this->botColor = botColor;
	currentColor = 2;

	board = new char*[8];

	for (int i = 0; i < 8; i++) {
		board[i] = new char[8];
	}
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			board[i][j] = 1;

	int color = 2;
	for (int i = 0; i < 8; i++) {
		for (int j = (i + 1) % 2; j < 8; j+=2) 
			board[i][j] = color;
		if (i == 2) {
			color = 3;
			i += 2;
		}
	}
}


BoardState::BoardState(Board& board) {
	botColor = 2;
	currentColor = 2;
	this->board = new char*[8];

	for (int i = 0; i < 8; i++) {
		this->board[i] = new char[8];
	}

	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			this->board[y][x] = board.board[y][x];
}

BoardState::BoardState(BoardState& bs, std::list<Eatable*> eaten, int x, int y, int dx, int dy, int currentColor) {
	this->board = new char*[8];

	for (int i = 0; i < 8; i++) {
		this->board[i] = new char[8];
	}
	for (int y = 0; y < 8; y++) 
		for (int x = 0; x < 8; x++) 
			this->board[y][x] = bs.board[y][x];
	this->currentColor = currentColor;
	botColor = 2;
	this->eat = !eaten.empty();

	movableX = x; movableY = y;
	moveX = x + dx; moveY = y + dy;
	if (!eat)
		addMove();
	else
		this->previousEaten = move(eaten);
}

std::vector<BoardState*>* BoardState::getPossibleMoves(){
	std::vector<BoardState*>* result = getEatableMoves();

	if (result->empty())
		result = getMoves();

	return result;
}

std::vector<BoardState*>* BoardState::getEatableMoves() {
	std::vector<BoardState*>* result = new std::vector<BoardState*>();
	result->reserve(10);

	int enemy = currentColor == 2 ? 3 : 2;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			curX = x; curY = y;
			if (currentColor == board[y][x]) {
				if (y < 6 && x < 6 && (board[y + 1][x + 1] == enemy || board[y + 1][x + 1] == enemy + 2) && board[y + 2][x + 2] == 1)
					dealEatable(x, y, 1, 1, result);
				if (y < 6 && x > 1 && (board[y + 1][x - 1] == enemy || board[y + 1][x - 1] == enemy + 2) && board[y + 2][x - 2] == 1)
					dealEatable(x, y, -1, 1, result);
				if (y > 1 && x < 6 && (board[y - 1][x + 1] == enemy || board[y - 1][x + 1] == enemy + 2) && board[y - 2][x + 2] == 1)
					dealEatable(x, y, 1, -1, result);
				if (y > 1 && x > 1 && (board[y - 1][x - 1] == enemy || board[y - 1][x - 1] == enemy + 2) && board[y - 2][x - 2] == 1)
					dealEatable(x, y, -1, -1, result);
			}
			else if (currentColor + 2 == board[y][x]) {
				int i;
				for (i = 1; y + i < 6 && x + i < 6 && board[y + i][x + i] == 1; i++);
				if (y + i < 7 && x + i < 7 && (board[y + i][x + i] == enemy || board[y + i][x + i] == enemy + 2))
					for (int j = i + 1; y + j < 8 && x + j < 8 && board[y + j][x + j] == 1; j++)
						dealEatableQueen(x, y, x + i, y + i, x + j, y + j, result);

				for (i = 1; y + i < 6 && x - i > 1 && board[y + i][x - i] == 1; i++);
				if (y + i < 7 && x - i > 0 && (board[y + i][x - i] == enemy || board[y + i][x - i] == enemy + 2))
					for (int j = i + 1; y + j < 8 && x - j > -1 && board[y + j][x - j] == 1; j++)
						dealEatableQueen(x, y, x - i, y + i, x - j, y + j, result);

				for (i = 1; y - i > 1 && x + i < 6 && board[y - i][x + i] == 1; i++);
				if (y - i > 0 && x + i < 7 && (board[y - i][x + i] == enemy || board[y - i][x + i] == enemy + 2))
					for (int j = i + 1; y - j > -1 && x + j < 8 && board[y - j][x + j] == 1; j++)
						dealEatableQueen(x, y, x + i, y - i, x + j, y - j, result);

				for (i = 1; y - i > 1 && x - i > 1 && board[y - i][x - i] == 1; i++);
				if (y - i > 0 && x - i > 0 && (board[y - i][x - i] == enemy || board[y - i][x - i] == enemy + 2))
					for (int j = i + 1; y - j > -1 && x - j > -1 && board[y - j][x - j] == 1; j++)
						dealEatableQueen(x, y, x - i, y - i, x - j, y - j, result);
			}
		}
	}

	return result;
}
std::vector<BoardState*>* BoardState::getMoves() {
	std::vector<BoardState*>* result = new std::vector<BoardState*>();
	result->reserve(10);

	int enemy = currentColor == 2 ? 3 : 2;
	int dy = currentColor == 2 ? 1 : -1;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (currentColor == board[y][x]) {
				if (board[y + dy][x - 1] == 1)
					result->push_back(new BoardState(*this, eaten, x, y, -1, dy, enemy));
				if (board[y + dy][x + 1] == 1)
					result->push_back(new BoardState(*this, eaten, x, y, +1, dy, enemy));
			}
			else if (currentColor + 2 == board[y][x]) {
				for (int i = 1; y + i < 8 && x + i < 8 && board[y + i][x + i] == 1; i++)
					result->push_back(new BoardState(*this, eaten, x, y, i, i, enemy));
				for (int i = 1; y + i < 8 && x - i > -1 && board[y + i][x - i] == 1; i++)
					result->push_back(new BoardState(*this, eaten, x, y, -i, i, enemy));
				for (int i = 1; y - i > -1 && x + i < 8 && board[y - i][x + i] == 1; i++)
					result->push_back(new BoardState(*this, eaten, x, y, i, -i, enemy));
				for (int i = 1; y - i > -1 && x - i > -1 && board[y - i][x - i] == 1; i++)
					result->push_back(new BoardState(*this, eaten, x, y, -i, -i, enemy));
			}
		}
	}

	return result;
}

void BoardState::dealEatable(int x, int y, int dx, int dy, std::vector<BoardState*>* result) {
	eaten.push_back(new Eatable(x + dx, y + dy, x + 2 * dx, y + 2 * dy));

	std::vector<BoardState*>* temp = recursEat(x, y, dx, dy);
	result->reserve(temp->size() + result->size());
	result->insert(result->end(), temp->begin(), temp->end());

	eaten.pop_back();
}
void BoardState::dealEatableQueen(int x, int y, int enemyX, int enemyY, int moveX, int moveY, std::vector<BoardState*>* result) {
	eaten.push_back(new Eatable(enemyX, enemyY, moveX, moveY));

	std::vector<BoardState*>* temp = recursEatQueen(x, y, enemyX, enemyY, moveX, moveY);
	result->reserve(temp->size() + result->size());
	result->insert(result->end(), temp->begin(), temp->end());

	eaten.pop_back();
}

std::vector<BoardState*>* BoardState::recursEat(int x, int y, int dx, int dy) {
	std::vector<BoardState*>* result = new std::vector<BoardState*>();
	int enemy = currentColor == 2 ? 3 : 2,
	    cx = x + 2 * dx, //current x
		cy = y + 2 * dy; //current y

	bool isQ = board[y + dy][x + dx] == enemy ? false : true;

	board[y][x] = 1;
	board[y + dy][x + dx] = 1;
	if (cy == 0 && currentColor == 3 || cy == 7 && currentColor == 2)
		board[cy][cx] = currentColor + 2;
	else
		board[cy][cx] = currentColor;
	

	if (cy < 6 && cx < 6 && (board[cy + 1][cx + 1] == enemy || board[cy + 1][cx + 1] == enemy + 2) && board[cy + 2][cx + 2] == 1)
		dealEatable(cx, cy, 1, 1, result);
	if (cy < 6 && cx > 1 && (board[cy + 1][cx - 1] == enemy || board[cy + 1][cx - 1] == enemy + 2) && board[cy + 2][cx - 2] == 1)
		dealEatable(cx, cy, -1, 1, result);
	if (cy > 1 && cx < 6 && (board[cy - 1][cx + 1] == enemy || board[cy - 1][cx + 1] == enemy + 2) && board[cy - 2][cx + 2] == 1)
		dealEatable(cx, cy, 1, -1, result);
	if (cy > 1 && cx > 1 && (board[cy - 1][cx - 1] == enemy || board[cy - 1][cx - 1] == enemy + 2) && board[cy - 2][cx - 2] == 1)
		dealEatable(cx, cy, -1, -1, result);

	if (result->empty())
		result->push_back(new BoardState(*this, eaten, curX, curY, 0, 0, enemy));

	board[y][x] = currentColor;
	board[y + dy][x + dx] = isQ ? enemy + 2 : enemy;
	board[cy][cx] = 1;

	return result;
}
std::vector<BoardState*>* BoardState::recursEatQueen(int xStart, int yStart, int enemyX, int enemyY, int x, int y){
	std::vector<BoardState*>* result = new std::vector<BoardState*>();
	int enemy = currentColor == 2 ? 3 : 2;
	bool isQ = board[enemyY][enemyX] == enemy ? false : true;
	int closeDx = x - xStart > 0 ? -1 : 1, closeDy = y - yStart > 0 ? -1 : 1;

	board[yStart][xStart] = 1;
	board[enemyY][enemyX] = 1;
	board[y][x] = currentColor + 2;

	int i;
	for (i = 1; y + i < 6 && x + i < 6 && board[y + i][x + i] == 1; i++);
	if (closeDx != 1 && closeDy != 1 && y + i < 7 && x + i < 7 && (board[y + i][x + i] == enemy || board[y + i][x + i] == enemy + 2))
		for (int j = i + 1; y + j < 8 && x + j < 8 && board[y + j][x + j] == 1; j++)
			dealEatableQueen(x, y, x + i, y + i, x + j, y + j, result);

	for (i = 1; y + i < 6 && x - i > 1 && board[y + i][x - i] == 1; i++);
	if (closeDx != -1 && closeDy != 1 && y + i < 7 && x - i > 0 && (board[y + i][x - i] == enemy || board[y + i][x - i] == enemy + 2))
		for (int j = i + 1; y + j < 8 && x - j > -1 && board[y + j][x - j] == 1; j++)
			dealEatableQueen(x, y, x - i, y + i, x - j, y + j, result);

	for (i = 1; y - i > 1 && x + i < 6 && board[y - i][x + i] == 1; i++);
	if (closeDx != 1 && closeDy != -1 && y - i > 0 && x + i < 7 && (board[y - i][x + i] == enemy || board[y - i][x + i] == enemy + 2))
		for (int j = i + 1; y - j > -1 && x + j < 8 && board[y - j][x + j] == 1; j++)
			dealEatableQueen(x, y, x + i, y - i, x + j, y - j, result);

	for (i = 1; y - i > 1 && x - i > 1 && board[y - i][x - i] == 1; i++);
	if (closeDx != -1 && closeDy != -1 && y - i > 0 && x - i > 0 && (board[y - i][x - i] == enemy || board[y - i][x - i] == enemy + 2))
		for (int j = i + 1; y - j > -1 && x - j > -1 && board[y - j][x - j] == 1; j++)
			dealEatableQueen(x, y, x - i, y - i, x - j, y - j, result);

	if (result->empty())
		result->push_back(new BoardState(*this, eaten, curX, curY, 0, 0, enemy));

	board[yStart][xStart] = currentColor + 2;
	board[enemyY][enemyX] = isQ ? enemy + 2 : enemy;
	board[y][x] = 1;

	return result;
}


void BoardState::addMove() {
	int enemy = currentColor == 2 ? 3 : 2;

	if (board[movableY][movableX] == enemy + 2)
		board[moveY][moveX] = enemy + 2;
	else if (moveY == 0 || moveY == 7)
		board[moveY][moveX] = enemy + 2;
	else
		board[moveY][moveX] = enemy;
	board[movableY][movableX] = 1;
}

int BoardState::getBotCount() {
	int result = 0;
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			if (board[y][x] == botColor || board[y][x] == botColor + 2) result++;
	return result;
}
int BoardState::getPlayerCount() {
	int result = 0, playerColor = botColor == 2 ? 3 : 2;
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			if (board[y][x] == playerColor || board[y][x] == playerColor + 2) result++;
	return result;
}

int BoardState::getHeuristics(int bCount, int pCount) {
	int botCount = 0,
		playerCount = 0,
		botQueen = 0,
		playerQueen = 0,
		playerColor = botColor == 2 ? 3 : 2;

	int result = 0;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (board[y][x] == botColor) { botCount++; result += abs(4 - y); }
			if (board[y][x] == playerColor) { playerCount++; result -= abs(4 - y); }
			if (board[y][x] == botColor + 2) botQueen++;
			if (board[y][x] == playerColor + 2) playerQueen++;
		}
	}
	if (playerCount == 0 && playerQueen == 0)
		return 5000;
	if (botCount == 0 && botQueen == 0)
		return -5000;


	result += 3 * ((pCount - playerCount - playerQueen) - (bCount - botCount - botQueen)) + 20 * (botQueen - playerQueen);

	return result;
}

bool BoardState::operator==(BoardState& bs) {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (board[i][j] != bs.board[i][j])
				return false;
	return true;
}

BoardState::~BoardState(){
	for (int i = 0; i < 8; i++)
		delete[]board[i];
	delete board;
}
