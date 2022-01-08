#include <iostream>

#include <Windows.h>

#include "Fields/SquareField.h"
#include "GameMode/GameMode.h"

using namespace std;

int main(int argc, char** argv)
{
	fields::SquareField field(4);

	field.generate();

	game_mode::GameMode game(field, { "�����", "���������" });

	SetConsoleOutputCP(1251);

	cout << "����� ������� ���� ����� ���������� (0, 0)" << endl << endl;

	game.playGame();

	return 0;
}
