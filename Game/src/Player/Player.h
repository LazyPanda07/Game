#pragma once

#include <iostream>

#include "Fields/BaseField.h"

namespace player
{
	class Player
	{
	private:
		std::vector<std::pair<size_t, size_t>> path;
		std::vector<std::pair<size_t, size_t>> possiblePath;
		std::string name;
		std::vector<int64_t> color;
		fields::BaseField& field;
		size_t currentX;
		size_t currentY;
		bool isSkipPreviousTurn;
		size_t allFieldPositionCount;
		size_t turns;

	public:
		Player(const std::string& playerName, const std::vector<int64_t>& color, fields::BaseField& field);

		Player(const Player& other);

		Player& operator = (const Player& other);

		void calculatePossiblePath(const Player& opponent);

		void makeTurn(const Player& opponent);

		void printPath(std::ostream& stream) const;

		bool isWin() const;

		bool operator == (const Player& other) const;

		const std::string& getName() const;

		size_t getCurrentX() const;

		size_t getCurrentY() const;

		const std::vector<int64_t>& getColor() const;

		const std::vector<std::pair<size_t, size_t>>& getPossiblePath() const;

		~Player() = default;
	};
}
