#include "Player.h"

#include <format>

#include "JSONUtility.h"
#include "settings.h"
#include "Utility.h"

using namespace std;

namespace player
{
	Player::Player(const string& playerName, const vector<int64_t>& color, fields::BaseField& field) :
		name(playerName),
		color(color),
		field(field),
		isSkipPreviousTurn(false),
		turns(1)
	{
		currentX = startPosition.first;
		currentY = startPosition.second;

		path.push_back(startPosition);

		allFieldPositionCount = field.getAllPositionCount();
	}

	Player::Player(const Player& other) :
		path(other.path),
		name(other.name),
		color(other.color),
		field(other.field),
		currentX(other.currentX),
		currentY(other.currentY),
		isSkipPreviousTurn(other.isSkipPreviousTurn),
		allFieldPositionCount(other.allFieldPositionCount),
		turns(other.turns)
	{

	}

	Player& Player::operator = (const Player& other)
	{
		path = other.path;
		name = other.name;
		color = other.color;
		field = other.field;
		currentX = other.currentX;
		currentY = other.currentY;
		isSkipPreviousTurn = other.isSkipPreviousTurn;
		allFieldPositionCount = other.allFieldPositionCount;
		turns = other.turns;

		return *this;
	}

	void Player::calculatePossiblePath(const Player& opponent)
	{
		size_t pathSize = allFieldPositionCount / 2 - path.size();

		possiblePath = field.calculatePossiblePath(currentX, currentY, pathSize + 1, opponent.getPossiblePath());
	}

	void Player::makeTurn(const Player& opponent)
	{
		auto moveToNext = [this](pair<size_t, size_t>&& nextPosition)
		{
			currentX = nextPosition.first;
			currentY = nextPosition.second;

			path.push_back(move(nextPosition));

			field.fillPosition(currentX, currentY, turns++, color);
		};

		if (isSkipPreviousTurn)
		{
			pair<size_t, size_t> nextPositon = field.getNextTurnAfterSkip(currentX, currentY);

			isSkipPreviousTurn = false;

			moveToNext(move(nextPositon));

			this->calculatePossiblePath(opponent);

			return;
		}

		if (possiblePath.empty())
		{
			isSkipPreviousTurn = true;

			return;
		}

		if (get<bool>(isOpponentPathAround({ currentX, currentY }, opponent.getPossiblePath())))
		{
			this->calculatePossiblePath(opponent);
		}

		pair<size_t, size_t>& nextPosition = possiblePath.front();

		if (field[nextPosition.second][nextPosition.first] > 0)
		{
			this->calculatePossiblePath(opponent);

			this->makeTurn(opponent);
		}
		else
		{
			moveToNext(move(nextPosition));

			possiblePath.erase(possiblePath.begin());
		}
	}

	void Player::printPath(ostream& stream) const
	{
		stream << json::utility::toUTF8JSON("Путь игрока ", 1251) << name << ':' << endl;

		for (const auto& [x, y] : path)
		{
			stream << format("({}, {})", x, y) << endl;
		}

		stream << endl;
	}

	bool Player::isWin() const
	{
		return (path.size() - 1) == allFieldPositionCount / 2;
	}

	bool Player::operator == (const Player& other) const
	{
		return name == other.name;
	}

	const string& Player::getName() const
	{
		return name;
	}

	size_t Player::getCurrentX() const
	{
		return currentX;
	}

	size_t Player::getCurrentY() const
	{
		return currentY;
	}

	const vector<int64_t>& Player::getColor() const
	{
		return color;
	}

	const vector<pair<size_t, size_t>>& Player::getPossiblePath() const
	{
		return possiblePath;
	}
}
