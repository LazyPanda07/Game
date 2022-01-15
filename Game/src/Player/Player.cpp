#include "Player.h"

#include "JSONUtility.h"
#include "settings.h"

using namespace std;

namespace player
{
	Player::Player(const string& playerName, const vector<int64_t>& color, fields::BaseField& field) :
		name(playerName),
		color(color),
		field(field),
		isSkipPreviousTurn(false),
		allFieldPositionCount(field.getAllPositionCount()),
		turns(1)
	{
		currentX = startPosition.first;
		currentY = startPosition.second;

		path.push_back(startPosition);

		field.fillPosition(currentX, currentY, -2, color);
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

	void Player::calculatePossiblePath()
	{
		size_t pathSize = field.getAllPositionCount() / 2 - path.size();

		possiblePath = field.calculatePossiblePath(currentX, currentY, pathSize + 1);
	}

	void Player::makeTurn()
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

			this->calculatePossiblePath();

			return;
		}

		if (possiblePath.empty())
		{
			isSkipPreviousTurn = true;

			return;
		}

		pair<size_t, size_t>& nextPosition = possiblePath.front();

		if (field[nextPosition.second][nextPosition.first] > 0)
		{
			this->calculatePossiblePath();

			this->makeTurn();
		}
		else
		{
			moveToNext(move(nextPosition));

			possiblePath.erase(possiblePath.begin());
		}
	}

	void Player::printPath(ostream& stream) const
	{
		stream << json::utility::toUTF8JSON("���� ������ ", 1251) << name << ':' << endl;

		for (const auto& [x, y] : path)
		{
			stream << '(' << x << ", " << y << ')' << endl;
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
}
