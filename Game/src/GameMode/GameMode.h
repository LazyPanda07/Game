#pragma once

#include "Player/Player.h"
#include "JSONUtility.h"

namespace game_mode
{
	class GameMode
	{
	private:
		std::vector<player::Player> players;
		fields::BaseField& field;

	public:
		GameMode(fields::BaseField& field, const std::vector<json::utility::objectSmartPointer<json::utility::jsonObject>>& players);

		bool playGame();

		~GameMode() = default;
	};
}
