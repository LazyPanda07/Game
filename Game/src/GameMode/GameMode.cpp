#include "GameMode.h"

#include <algorithm>

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

	void GameMode::playGame()
	{
		const player::Player* winPlayer = nullptr;

		while (true)
		{
			if (field.isFieldFull())
			{
				cout << "Ни один игрок не смог выиграть так как все поле заполнено" << endl;

				return;
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

		cout << "Игрок " << winPlayer->getName() << " выиграл." << endl;

		winPlayer->printPath(cout);

		players.erase(find(players.begin(), players.end(), *winPlayer));

		for_each(players.begin(), players.end(), [](const player::Player& player) { player.printPath(cout); });
	}
}
