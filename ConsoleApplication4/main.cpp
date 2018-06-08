#include <SFML\Graphics.hpp>
#include <iostream>
#include <conio.h>
#include <time.h>
#include "GameDispatcher.h"
#include <Windows.h>

int main()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	//std::cout << sizeof(sf::Color) << " " << sizeof(sf::String) << " " << sizeof(BoardState) << " " << sizeof(Checker) << " " << sizeof(std::vector<BoardState*>);
	//_getch();

	srand(time(NULL));

	GameDispatcher gameDis(640, 640, 300, 50);

	gameDis.launchMapLoading();

	
	return 0;
}