#include "Player.h"

using namespace std;

namespace player
{
	Player::Player(const string& playerName, fields::BaseField& field) :
		name(playerName),
		field(field),
		isSkipPreviousTurn(false),
		allFieldPositionCount(field.getAllPositionCount())
	{
		pair<size_t, size_t> startPosition = field.setPlayerPosition();

		currentX = startPosition.first;
		currentY = startPosition.second;

		path.push_back(move(startPosition));

		field[currentY][currentX] = true;
	}

	Player::Player(const Player& other) :
		path(other.path),
		name(other.name),
		field(other.field),
		currentX(other.currentX),
		currentY(other.currentY),
		isSkipPreviousTurn(other.isSkipPreviousTurn),
		allFieldPositionCount(other.allFieldPositionCount)
	{

	}

	Player& Player::operator = (const Player& other)
	{
		path = other.path;
		name = other.name;
		field = other.field;
		currentX = other.currentX;
		currentY = other.currentY;
		isSkipPreviousTurn = other.isSkipPreviousTurn;
		allFieldPositionCount = other.allFieldPositionCount;

		return *this;
	}

	void Player::makeTurn()
	{
		pair<size_t, size_t> nextPositon = field.getAvailableNextTurnPosition(currentX, currentY, isSkipPreviousTurn);

		if (nextPositon == make_pair(currentX, currentY))
		{
			isSkipPreviousTurn = true;
		}
		else
		{
			isSkipPreviousTurn = false;

			currentX = nextPositon.first;
			currentY = nextPositon.second;

			path.push_back(move(nextPositon));

			field[currentY][currentX] = true;
		}
	}

	void Player::printPath(ostream& stream) const
	{
		stream << "Путь игрока " << name << ':' << endl;

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
}
