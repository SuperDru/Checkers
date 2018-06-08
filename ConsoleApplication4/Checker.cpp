#include "Checker.h"


void Checker::setKing(){
	king = true;
	sprite.setTexture(&(color == sf::Color::Black ? blackQueen : whiteQueen));
}

Checker::Checker(sf::Color color, int x, int y, int radius):
	color(color), x(x), y(y), radius(radius), king(false) {
	blackChk.loadFromFile("blackChk.png");
	whiteChk.loadFromFile("whiteChk.png");
	blackChkHighlited.loadFromFile("blackChkHighlited.png");
	whiteChkHighlited.loadFromFile("whiteChkHighlited.png");
	blackQueen.loadFromFile("blackQueen.png");
	whiteQueen.loadFromFile("whiteQueen.png");
	blackQueenHighlited.loadFromFile("blackQueenHighlited.png");
	whiteQueenHighlited.loadFromFile("whiteQueenHighlited.png");

	blackChk.setSmooth(true);
	whiteChk.setSmooth(true);
	blackQueen.setSmooth(true);
	whiteQueen.setSmooth(true);

	blackChkHighlited.setSmooth(true);
	whiteChkHighlited.setSmooth(true);
	blackQueenHighlited.setSmooth(true);
	whiteQueenHighlited.setSmooth(true);

	sprite.setTexture(&(color == sf::Color::Black ? blackChk : whiteChk));
}

void Checker::move(int dx, int dy, int dif){
	x += dx * dif;
	y += dy * dif;
	if (color == sf::Color::Black && y == 7 || color != sf::Color::Black && y == 0)
		setKing();
}


sf::CircleShape Checker::getRenderingChecker() {
	sprite.setRadius(radius * 7 / 8);
	sprite.setPosition(radius * (16 * x + 1) / 8, radius * (16 * (7 - y) + 1) / 8);

	return sprite;
}

void Checker::reset(){
	x = -1;
	y = -1;
}

void Checker::setHighlighted(){
	if (king)
		sprite.setTexture(&(color == sf::Color::Black ? blackQueenHighlited : whiteQueenHighlited));
	else
		sprite.setTexture(&(color == sf::Color::Black ? blackChkHighlited : whiteChkHighlited));
}
void Checker::resetHighlighted(){
	if (king)
		setKing();
	else
		sprite.setTexture(&(color == sf::Color::Black ? blackChk : whiteChk));
}

int Checker::getX(){
	return x;
}
int Checker::getY(){
	return y;
}
sf::Color Checker::getColor(){
	return color;
}

bool Checker::isKing(){
	return king;
}

Checker::~Checker(){}
