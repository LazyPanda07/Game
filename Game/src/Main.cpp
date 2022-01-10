#include <iostream>
#include <filesystem>
#include <fstream>

#include <Windows.h>

#include "Fields/SquareField.h"
#include "Fields/CircleField.h"
#include "Fields/QuadrantField.h"
#include "Fields/RectangleField.h"
#include "GameMode/GameMode.h"
#include "settings.h"

#include "JSONParser.h"

#pragma comment (lib, "JSON.lib")

using namespace std;

template<typename FieldT, typename... Args>
void startGame(const Args&... args);

bool initialization();

int main(int argc, char** argv)
{
	if (filesystem::exists("settings.json"))
	{
		ifstream("settings.json") >> settings;

		isInitialized = true;
	}
	else
	{
		isInitialized = false;
	}

	SetConsoleOutputCP(1251);

	try
	{
		if (!initialization())
		{
			startGame<fields::RectangleField>(10, 5);
		}
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

template<typename FieldT, typename... Args>
void startGame(const Args&... args)
{
	unique_ptr<FieldT> field = make_unique<FieldT>(args...);
	vector<string> players = { "Синий", "Оранжевый" };

	field->generate();

	unique_ptr<game_mode::GameMode> game = make_unique<game_mode::GameMode>(*(field.get()), players);

	while (!game->playGame() && isInitialized && settings.getBool("repeatAfterDraw"))
	{
		field = make_unique<FieldT>(args...);

		field->generate();

		game = make_unique<game_mode::GameMode>(*(field.get()), players);
	}
}

bool initialization()
{
	using namespace fields;

	if (isInitialized)
	{
		const auto& field = settings.getObject("field");
		const string& type = field->getString("type");

		if (type == "rectangle")
		{
			startGame<RectangleField>(field->getInt("width"), field->getInt("height"));
		}
		else if (type == "square")
		{
			startGame<SquareField>(field->getInt("size"));
		}
		else if (type == "circle")
		{
			startGame<CircleField>(field->getInt("radius"));
		}
		else if (type == "quadrant")
		{
			startGame<QuadrantField>(field->getInt("radius"));
		}
		else
		{
			throw runtime_error("Неверный тип поля");
		}
	}

	return isInitialized;
}
