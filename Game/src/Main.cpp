#include <iostream>

#include <Windows.h>

#include "Fields/SquareField.h"
#include "GameMode/GameMode.h"

using namespace std;

int main(int argc, char** argv)
{
	fields::SquareField field(4);

	field.generate();

	game_mode::GameMode game(field, { "Синий", "Оранжевый" });

	SetConsoleOutputCP(1251);

	game.playGame();

	return 0;
}
