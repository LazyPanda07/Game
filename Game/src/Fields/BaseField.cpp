#include "BaseField.h"

#include <random>
#include <ctime>
#include <algorithm>
#include <numeric>

using namespace std;

namespace fields
{
	bool BaseField::checkPosition(size_t x, size_t y) const
	{
		if (y < field.size())
		{
			if (x < field[y].size() && !field[y][x])
			{
				return true;
			}
		}

		return false;
	}

	void BaseField::findPath(vector<pair<size_t, size_t>>& possiblePath, size_t pathSize)
	{
		if (possiblePath.empty())
		{
			return;
		}

		auto recursiveFindPath = [this, &possiblePath, &pathSize](size_t x, size_t y)
		{
			pair<size_t, size_t> position = { x, y };
			auto checkDifference = [&possiblePath, &position]()
			{
				int64_t checkX = abs(static_cast<int64_t>(possiblePath.back().first) - static_cast<int64_t>(position.first));
				int64_t checkY = abs(static_cast<int64_t>(possiblePath.back().second) - static_cast<int64_t>(position.second));

				return checkX + checkY < 2;
			};

			if (possiblePath.size() - 1 == pathSize)
			{
				return;
			}

			if (this->checkPosition(x, y) && find(possiblePath.begin(), possiblePath.end(), position) == possiblePath.end() && checkDifference())
			{
				possiblePath.push_back(move(position));

				this->findPath(possiblePath, pathSize);
			}
		};

		const auto& [playerX, playerY] = possiblePath.back();

		recursiveFindPath(playerX + 1, playerY);

		recursiveFindPath(playerX, playerY + 1);

		if (playerX)
		{
			recursiveFindPath(playerX - 1, playerY);
		}

		if (playerY)
		{
			recursiveFindPath(playerX, playerY - 1);
		}
	}

	BaseField::BaseField(size_t width, size_t height) :
		width(width),
		height(height)
	{

	}

	void BaseField::generate()
	{
		field = const_cast<const BaseField*>(this)->generateField();
	}

	vector<pair<size_t, size_t>> BaseField::calculatePossiblePath(size_t currentX, size_t currentY, size_t pathSize)
	{
		vector<pair<size_t, size_t>> result = { make_pair(currentX, currentY) };

		this->findPath(result, pathSize);

		result.erase(result.begin());

		return result;
	}

	pair<size_t, size_t> BaseField::setPlayerPosition()
	{
		mt19937 random(static_cast<uint32_t>(time(nullptr)));

		size_t x = random() % width;
		size_t y = random() % height;

		if (this->checkPosition(x, y))
		{
			field[y][x] = false;

			return { x, y };
		}

		for (size_t y = 0; y < field.size(); y++)
		{
			for (size_t x = 0; x < field[y].size(); x++)
			{
				if (this->checkPosition(x, y))
				{
					field[y][x] = false;

					return { x, y };
				}
			}
		}

		return { 0, 0 };
	}

	pair<size_t, size_t> BaseField::getAvailableNextTurnPosition(size_t playerX, size_t playerY, bool isSkipPreviousTurn) const
	{
		if (isSkipPreviousTurn)
		{
			mt19937 random(static_cast<uint32_t>(time(nullptr)));

			size_t x = random() % width;
			size_t y = random() % height;

			if (this->checkPosition(x, y))
			{
				return { x, y };
			}

			for (size_t column = 0; column < field.size(); column++)
			{
				for (size_t row = 0; row < field[column].size(); row++)
				{
					if (field[column][row])
					{
						return { row, column };
					}
				}
			}
		}

		pair<size_t, size_t > result = { playerX, playerY };

		if (this->checkPosition(playerX + 1, playerY))
		{
			result = { playerX + 1, playerY };
		}
		else if (this->checkPosition(playerX, playerY + 1))
		{
			result = { playerX, playerY + 1 };
		}
		else if (playerX && this->checkPosition(playerX - 1, playerY))
		{
			result = { playerX - 1, playerY };
		}
		else if (playerY && this->checkPosition(playerX, playerY - 1))
		{
			result = { playerX, playerY - 1 };
		}

		return result;
	}

	bool BaseField::isFieldFull() const
	{
		return all_of(field.begin(), field.end(), [](const vector<bool>& row) { return all_of(row.begin(), row.end(), [](bool position) { return position; }); });
	}

	vector<bool>& BaseField::operator[](size_t index)
	{
		return field[index];
	}

	const vector<bool>& BaseField::operator [] (size_t index) const
	{
		return field[index];
	}

	size_t BaseField::getWidth() const
	{
		return width;
	}

	size_t BaseField::getHeight() const
	{
		return height;
	}

	size_t BaseField::getAllPositionCount() const
	{
		size_t result = 0;

		for (const auto& i : field)
		{
			result += i.size();
		}

		return result;
	}
}
