#pragma once

#include <iostream>

#include "Fields/BaseField.h"

namespace player
{
	class Player
	{
	private:
		std::vector<std::pair<size_t, size_t>> path;
		std::string name;
		fields::BaseField& field;
		size_t currentX;
		size_t currentY;
		bool isSkipPreviousTurn;
		size_t allFieldPositionCount;

	public:
		Player(const std::string& playerName, fields::BaseField& field);

		Player(const Player& other);

		Player& operator = (const Player& other);

		void makeTurn();

		void printPath(std::ostream& stream) const;

		bool isWin() const;

		bool operator == (const Player& other) const;

		const std::string& getName() const;

		size_t getCurrentX() const;

		size_t getCurrentY() const;

		~Player() = default;
	};
}
