#pragma once
struct Eatable {
	char x, y, moveX, moveY;

	Eatable(int x, int y, int moveX, int moveY) 
		: x(x), y(y), moveX(moveX), moveY(moveY) {};
};
