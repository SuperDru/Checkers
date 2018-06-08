#include "Board.h"
#include "BoardState.h"


Board::Board(unsigned width, unsigned height):
	width(width), height(height) {
	sprite.create(width, height);
	
	blackSpr.loadFromFile("BackgroundChkBlack.png");
	whiteSpr.loadFromFile("BackgroundChk.png");
	blackSpr.setSmooth(true);
	whiteSpr.setSmooth(true);


	//boardRect.setSize(sf::Vector2f(width / 8, height / 8));
	checkers.reserve(24);
	movableChk = nullptr;
	who = sf::Color(230, 230, 230);
	//who = sf::Color::Black;
	render();
}

sf::Sprite Board::getRenderingBoard() {
	renderBoard();
	renderCheckers();
	return sf::Sprite(sprite.getTexture());
}

bool Board::makeAction(sf::Vector2i pos) {
	if (movableChk == nullptr) 
		setMovable(pos.x / (width / 8), pos.y / (width / 8));
	else if (!eat && move(pos.x, pos.y)) {
		renderBoard();
		renderCheckers();
		eat = isNeedEat();
		return true;
	}
	else if (eat && tryEat(pos)) {
		return true;
	}
	return false;
}

void Board::render() {
	sf::Color dotColor;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			spr.setTexture(board[i][j] == 0 ? whiteSpr : blackSpr);
			spr.setPosition(j * width / 8, (7 - i) * height / 8);
			sprite.draw(spr);

			Checker *chk;
			if (board[i][j] == 2 || board[i][j] == 4)
				chk = new Checker(sf::Color::Black, j, i, width / 16);
			else if (board[i][j] == 3 || board[i][j] == 5)
				chk = new Checker(sf::Color(230, 230, 230), j, i, width / 16);
			else continue;

			if (board[i][j] == 4 || board[i][j] == 5)
				chk->setKing();

			checkers.push_back(chk);
		}
	eat = isNeedEat();
}
void Board::renderBoard(){
	sf::Color dotColor;
	int c = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//if (board[i][j] == 0) dotColor = sf::Color(255, 230, 179);
			//else dotColor = sf::Color(128, 64, 0);

			spr.setTexture(board[i][j] == 0 ? whiteSpr : blackSpr);
			spr.setPosition(j * width / 8, (7 - i) * height / 8);
			sprite.draw(spr);

			board[i][j] = c++ % 2;

			//boardRect.setFillColor(dotColor);
			//boardRect.setPosition(j * width / 8, (7 - i) * height / 8);
			//sprite.draw(boardRect);
		}
		c++;
	}
}
void Board::renderCheckers(){
	for (int i = 0; i < checkers.size(); i++) {
		int xChk = checkers[i]->getX(),
			yChk = checkers[i]->getY();
		if (xChk != -1) {
			board[yChk][xChk] = checkers[i]->getColor() == sf::Color::Black ? 2 : 3;
			if (checkers[i]->isKing())
				board[yChk][xChk] += 2;
		}
		sf::CircleShape circle = checkers[i]->getRenderingChecker();
		sprite.draw(circle);
	}
}

void Board::resetMovable(){
	if (movableChk != nullptr) {
		movableChk->resetHighlighted();
		movableChk = nullptr;
	}
	who = who == sf::Color::Black ? sf::Color(230, 230, 230) : sf::Color::Black;
}
void Board::setMovable(int x, int y) {
	if (movableChk != nullptr)
		movableChk->resetHighlighted();
	movableChk = nullptr;
	for (auto chk : checkers) {
		if (x == chk->getX() && y == chk->getY() && who == chk->getColor()) {
			movableChk = chk;
			chk->setHighlighted();
		}
	}
}

bool Board::move(int x, int y){
	int xChk = movableChk->getX(),
		yChk = movableChk->getY(),
	    posX = x / (width / 8),
		posY = y / (width / 8);

	int dif = abs(posX - xChk),
		dx = posX - xChk > 0 ? 1 : -1, dy;
	if (movableChk->isKing()) dy = posY - yChk > 0 ? 1 : -1;
	else dy = movableChk->getColor() == sf::Color::Black ? 1 : -1;
	do {
		xChk += dx; yChk += dy;
		if (xChk == posX && yChk == posY && board[yChk][xChk] == 1) {
			movableChk->move(dx, dy, dif);
			resetMovable();
			return true;
		}
	} while (movableChk->isKing() && board[yChk][xChk] == 1);
	setMovable(posX, posY);
	return false;
}
bool Board::makeMove(int x, int y, int moveX, int moveY) {
	for (int i = 0; i < checkers.size(); i++) 
		if (checkers[i]->getX() == x && checkers[i]->getY() == y) 
			checkers[i]->move(moveX - x, moveY - y, 1);
	renderBoard();
	renderCheckers();
	resetMovable();
	eat = isNeedEat();
	return true;
}
bool Board::eatChk(int x, int y, std::list<Eatable*> eaten) { 
	int moveX, moveY, movableX = x, movableY = y;

	int enemy = who == sf::Color::Black ? 2 : 3;

	for (auto && e : eaten) {
		//moveY = 2 * e->y - movableY;
		//moveX = 2 * e->x - movableX;

		for (auto && chk : checkers) {
			if (chk->getX() == movableX && chk->getY() == movableY)
				chk->move(e->moveX - movableX, e->moveY - movableY, 1);
			if (chk->getX() == e->x && chk->getY() == e->y)
				chk->reset();
		}

		movableY = e->moveY;
		movableX = e->moveX;
	}

	renderBoard();
	renderCheckers();
	resetMovable();

	eat = isNeedEat();
	return true; 
}

bool Board::tryEat(sf::Vector2i pos){
	int enemy = who == sf::Color::Black ? 3 : 2,
		posX = pos.x / (width / 8),
		posY = pos.y / (width / 8),
		x = movableChk->getX(),
		y = movableChk->getY(),
		dx = posX - x > 0 ? 1 : -1,
		dy = posY - y > 0 ? 1 : -1,
		dif = abs(posX - x);

	if (posX - x == 0 || posY - y == 0 || abs(posX - x) != abs(posY - y) || !movableChk->isKing() && dif > 2) {
		setMovable(posX, posY);
		return false;
	}

	int	tempPosX = x + 2 * dx,
		tempPosY = y + 2 * dy;

	int enemyX, enemyY;

	bool f = false, eaten = false;

	for (;!f; x+=dx, y+=dy, tempPosX += dx, tempPosY += dy) {
		if (tempPosX == posX && tempPosY == posY) f = true;
		if (eaten && (board[y + dy][x + dx] != 1 || board[y + 2 * dy][x + 2 * dx] != 1 ) ||
			board[y + dy][x + dx] != 1 && board[y + 2 * dy][x + 2 * dx] != 1) {
			setMovable(posX, posY);
			return false;
		}
		if ((board[y + dy][x + dx] == enemy || board[y + dy][x + dx] == enemy + 2) && board[y + 2 * dy][x + 2 * dx] == 1) {
			enemyX = x + dx;
			enemyY = y + dy;
			eaten = true;
		}
	}

	if (eaten) {
		for (auto chk : checkers) {
			if (chk->getX() == enemyX && chk->getY() == enemyY) chk->reset();
		}

		movableChk->move(dx, dy, dif);
		renderBoard();
		renderCheckers();
		eat = movableChk->isKing() ? isNeedEatKing(movableChk) : isNeedEat(movableChk->getX(), movableChk->getY());
		if (!eat) {
			resetMovable();
			eat = isNeedEat();
			return true;
		}
	}
	else setMovable(posX, posY);
	return false;
}

bool Board::isNeedEat() {
	for (auto chk : checkers)
		if (chk->getColor() == who && (chk->isKing() && isNeedEatKing(chk) || !chk->isKing() && isNeedEat(chk->getX(), chk->getY())))
			return true;
	return false;
}

bool Board::isNeedEat(int xChk, int yChk){
	int enemy = who == sf::Color::Black ? 3 : 2;

	if (xChk < 6 && yChk > 1 && (board[yChk - 1][xChk + 1] == enemy || board[yChk - 1][xChk + 1] == enemy + 2) && board[yChk - 2][xChk + 2] == 1 ||
		xChk > 1 && yChk > 1 && (board[yChk - 1][xChk - 1] == enemy || board[yChk - 1][xChk - 1] == enemy + 2) && board[yChk - 2][xChk - 2] == 1 ||
		xChk < 6 && yChk < 6 && (board[yChk + 1][xChk + 1] == enemy || board[yChk + 1][xChk + 1] == enemy + 2) && board[yChk + 2][xChk + 2] == 1 ||
		xChk > 1 && yChk < 6 && (board[yChk + 1][xChk - 1] == enemy || board[yChk + 1][xChk - 1] == enemy + 2) && board[yChk + 2][xChk - 2] == 1)
		return true;
	return false;
}
bool Board::isNeedEatKing(Checker * chk){
	int enemy = who == sf::Color::Black ? 3 : 2;
	int x = chk->getX();
	int y = chk->getY();

	int i;
	for (i = 1; board[y + i][x + i] == 1; i++);
	if (board[y + i][x + i] == enemy || board[y + i][x + i] == enemy + 2)
		if (board[y + i + 1][x + i + 1] == 1) return true;

	for (i = 1; board[y + i][x - i] == 1; i++);
	if (board[y + i][x - i] == enemy || board[y + i][x - i] == enemy + 2)
		if (board[y + i + 1][x - i - 1] == 1) return true;

	for (i = 1; board[y - i][x + i] == 1; i++);
	if (board[y - i][x + i] == enemy || board[y - i][x + i] == enemy + 2)
		if (board[y - i - 1][x + i + 1] == 1) return true;

	for (i = 1; board[y - i][x - i] == 1; i++);
	if (board[y - i][x - i] == enemy || board[y - i][x - i] == enemy + 2)
		if (board[y - i - 1][x - i - 1] == 1) return true;

	return false;
}

Board::~Board(){}
