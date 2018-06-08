#include "GameDispatcher.h"
#include "Bot.h"


GameDispatcher::GameDispatcher(int width, int height, int x, int y):
	width(width), height(height) {
	window.create(sf::VideoMode(width, height), "Checkers", sf::Style::Close);
	window.setPosition(sf::Vector2i(x, y));
	width = window.getSize().x;
	height = window.getSize().y;
}

void GameDispatcher::launchMapLoading(){
	Board board(width, height);
	Bot bot("black");
	bool botMove = false;

	while (window.isOpen()) {
		sf::Vector2i pos = sf::Mouse::getPosition(window);

		if (botMove) {
			bot.makeMove(board);
			botMove = false;
		}

		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonReleased:
				if (e.key.code == sf::Mouse::Left) {
					if (board.makeAction(pos))
						botMove = true;
				}
				break;
			}
		}

		sf::Sprite sp = board.getRenderingBoard();
		
		window.clear();
		window.draw(sp);
		window.display();
	}
}


GameDispatcher::~GameDispatcher(){}
