#pragma once

#include "Player/Player.h"

namespace game_mode
{
	class GameMode
	{
	private:
		std::vector<player::Player> players;
		fields::BaseField& field;

	public:
		GameMode(fields::BaseField& field, const std::vector<std::string>& playersNames);

		bool playGame();

		~GameMode() = default;
	};
}
