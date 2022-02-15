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
#include "JSONArrayWrapper.h"

#pragma comment (lib, "JSON.lib")

#pragma warning(disable: 26800)

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

	SetConsoleOutputCP(CP_UTF8);

	try
	{
		if (!initialization())
		{
			cout << json::utility::toUTF8JSON("Запускается квадратное поле 4x4 с 2 игроками", 1251) << endl;

			startGame<fields::SquareField>(4);
		}
	}
	catch (const exception& e)
	{
		cout << e.what() << endl;
	}

#ifndef _DEBUG
	system("pause");
#endif // !_DEBUG
	return 0;
}

template<typename FieldT, typename... Args>
void startGame(const Args&... args)
{
	using json::utility::toUTF8JSON;

	FieldT field(args...);
	vector<json::utility::jsonObject> players;
	pair<size_t, size_t> startPosition = fields::BaseField::notInitializedPosition;

	if (isInitialized)
	{
		players = json::utility::JSONArrayWrapper(settings.getArray("players")).getAsObjectArray();

		try
		{
			const json::utility::jsonObject& object = settings.getObject("startPosition");

			startPosition = { object.getInt("x"), object.getInt("y") };
		}
		catch (const json::exceptions::CantFindValueException&)
		{

		}
	}
	else
	{
		json::utility::jsonObject blue;
		json::utility::jsonObject orange;
		vector<json::utility::jsonObject> color;

		json::utility::appendArray(0, color);
		json::utility::appendArray(0, color);
		json::utility::appendArray(255, color);

		blue.data.push_back({ "name"s, toUTF8JSON("Синий", 1251) });
		blue.data.push_back({ "color"s, move(color) });

		json::utility::appendArray(255, color);
		json::utility::appendArray(165, color);
		json::utility::appendArray(0, color);

		orange.data.push_back({ "name"s, toUTF8JSON("Оранжевый", 1251) });
		orange.data.push_back({ "color"s, move(color) });

		json::utility::appendArray(move(blue), players);
		json::utility::appendArray(move(orange), players);
	}

	field.generate();

	startPosition = field.setPlayerPosition(startPosition);

	game_mode::GameMode game(field, players);

	game.playGame();
}

bool initialization()
{
	using namespace fields;

	if (isInitialized)
	{
		const auto& field = settings.getObject("field");
		const string& type = field.getString("type");

		if (type == "rectangle")
		{
			startGame<RectangleField>(field.getInt("width"), field.getInt("height"));
		}
		else if (type == "square")
		{
			startGame<SquareField>(field.getInt("size"));
		}
		else if (type == "circle")
		{
			startGame<CircleField>(field.getInt("radius"));
		}
		else if (type == "quadrant")
		{
			startGame<QuadrantField>(field.getInt("radius"));
		}
		else
		{
			throw runtime_error(json::utility::toUTF8JSON("Неверный тип поля", 1251));
		}
	}

	return isInitialized;
}
