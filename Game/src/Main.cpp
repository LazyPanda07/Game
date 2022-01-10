#include <iostream>

#include <Windows.h>

#include "Fields/SquareField.h"
#include "Fields/CircleField.h"
#include "GameMode/GameMode.h"

using namespace std;

template<typename FieldT, typename... Args>
void startGame(Args&&... args);

int main(int argc, char** argv)
{
	SetConsoleOutputCP(1251);

	startGame<fields::CircleField>(5);

	return 0;
}

template<typename FieldT, typename... Args>
void startGame(Args&&... args)
{
	FieldT field(forward<Args>(args)...);

	field.generate();

	game_mode::GameMode game(field, { "Синий", "Оранжевый" });

	game.playGame();
}
