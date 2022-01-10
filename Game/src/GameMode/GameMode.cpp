#include "GameMode.h"

#include <iostream>
#include <fstream>
#include <algorithm>

#include "settings.h"

using namespace std;

namespace game_mode
{
	GameMode::GameMode(fields::BaseField& field, const vector<string>& playersNames) :
		field(field)
	{
		for (const auto& name : playersNames)
		{
			players.emplace_back(name, field);
		}
	}

	bool GameMode::playGame()
	{
		using json::utility::toUTF8JSON;

		const player::Player* winPlayer = nullptr;
		unique_ptr<ofstream> outFile;
		ostream& outputStream = [&outFile]() -> ostream&
		{
			if (isInitialized)
			{
				try
				{
					const string& outName = settings.getString("out");

					if (outName == "standard")
					{
						return cout;
					}

					outFile = make_unique<ofstream>(outName, ios::app);

					return *outFile;
				}
				catch (const json::exceptions::BaseJSONException&)
				{
					return cout;
				}
			}
			else
			{
				return cout;
			}
		}();

		for (auto& player : players)
		{
			player.calculatePossiblePath();
		}

		while (true)
		{
			if (field.isFieldFull())
			{
				if (isInitialized && settings.getBool("repeatAfterDraw"))
				{
					return false;
				}

				outputStream << toUTF8JSON("Ни один игрок не смог выиграть так как все поле заполнено", 1251) << endl;

				return false;
			}

			for (auto& player : players)
			{
				player.makeTurn();

				if (player.isWin())
				{
					winPlayer = &player;

					break;
				}
			}

			if (winPlayer)
			{
				break;
			}
		}

		outputStream << toUTF8JSON("Игрок ", 1251) << winPlayer->getName() << toUTF8JSON(" выиграл.", 1251) << endl;

		winPlayer->printPath(outputStream);

		players.erase(find(players.begin(), players.end(), *winPlayer));

		for_each(players.begin(), players.end(), [&outputStream](const player::Player& player) { player.printPath(outputStream); });

		return true;
	}
}
