#include "BaseField.h"

#include <random>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <stack>
#include <array>

#include "Utility.h"

using namespace std;

static mt19937_64 random(static_cast<uint32_t>(time(nullptr)));
static constexpr size_t triesToFindEmptyPosition = 100;

static size_t getDigits(int value);

namespace fields
{
	const pair<size_t, size_t> BaseField::notInitializedPosition = { -1, -1 };

	bool BaseField::checkPosition(size_t x, size_t y) const
	{
		if (y < field.size())
		{
			if (x < field[y].size() && field[y][x] == 0)
			{
				return true;
			}
		}

		return false;
	}

	void BaseField::findPath(const pair<size_t, size_t>& startPosition, map<size_t, vector<pair<size_t, size_t>>>& paths, size_t pathSize)
	{
		stack<vector<pair<size_t, size_t>>> availablePaths;
		auto appendPath = [this, &availablePaths, &paths](const vector<pair<size_t, size_t>>& currentPath, size_t x, size_t y)
		{
			pair<size_t, size_t> position = make_pair(x, y);

			if (this->checkPosition(position.first, position.second) && ranges::find(currentPath, position) == currentPath.end())
			{
				vector<pair<size_t, size_t>> nextPath = currentPath;

				nextPath.push_back(move(position));

				paths.insert(make_pair(nextPath.size(), nextPath));

				availablePaths.push(move(nextPath));
			}
		};

		availablePaths.push({ startPosition });

		while (availablePaths.size())
		{
			vector<pair<size_t, size_t>> currentPath = move(availablePaths.top());
			const auto& [x, y] = currentPath.back();

			availablePaths.pop();

			if (currentPath.size() - 1 == pathSize)
			{
				return;
			}

			appendPath(currentPath, x + 1, y);
			appendPath(currentPath, x, y + 1);

			if (x)
			{
				appendPath(currentPath, x - 1, y);
			}

			if (y)
			{
				appendPath(currentPath, x, y - 1);
			}
		}
	}

	BaseField::BaseField(size_t width, size_t height) :
		width(width),
		height(height),
		maxTurnDigits(0)
	{

	}

	void BaseField::generate()
	{
		field = const_cast<const BaseField*>(this)->generateField();
	}

	vector<pair<size_t, size_t>> BaseField::calculatePossiblePath(size_t currentX, size_t currentY, size_t pathSize)
	{
		map<size_t, vector<pair<size_t, size_t>>> paths;

		this->findPath(make_pair(currentX, currentY), paths, pathSize);

		if (paths.empty())
		{
			return {};
		}

		vector<pair<size_t, size_t>> result = move(paths.rbegin()->second);

		result.erase(result.begin());

		return result;
	}

	void BaseField::fillPosition(size_t x, size_t y, size_t turnNumber, const vector<int64_t>& color)
	{
		field[y][x] = static_cast<int>(turnNumber);

		maxTurnDigits = max(maxTurnDigits, getDigits(static_cast<int>(turnNumber)));

		colors[&field[y][x]] = color;
	}

	pair<size_t, size_t> BaseField::setPlayerPosition(const pair<size_t, size_t>& startPosition)
	{
		if (startPosition != notInitializedPosition)
		{
			if (this->checkPosition(startPosition.first, startPosition.second))
			{
				field[startPosition.second][startPosition.first] = -2;

				return startPosition;
			}
		}

		for (size_t i = 0; i < triesToFindEmptyPosition; i++)
		{
			size_t x = random() % width;
			size_t y = random() % height;

			if (this->checkPosition(x, y))
			{
				field[y][x] = -2;

				return { x, y };
			}
		}

		for (size_t y = 0; y < field.size(); y++)
		{
			for (size_t x = 0; x < field[y].size(); x++)
			{
				if (this->checkPosition(x, y))
				{
					field[y][x] = -2;

					return { x, y };
				}
			}
		}

		return { 0, 0 };
	}

	pair<size_t, size_t> BaseField::getNextTurnAfterSkip(size_t playerX, size_t playerY) const
	{
		for (size_t i = 0; i < triesToFindEmptyPosition; i++)
		{
			size_t x = random() % width;
			size_t y = random() % height;

			if (this->checkPosition(x, y))
			{
				return { x, y };
			}
		}

		for (size_t column = 0; column < field.size(); column++)
		{
			for (size_t row = 0; row < field[column].size(); row++)
			{
				if (field[column][row] == 0)
				{
					return { row, column };
				}
			}
		}

		return { 0, 0 };
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
			result += ranges::count_if(i, [](int available) { return available >= 0 || available == -2; });
		}

		return result;
	}

	const vector<int>& BaseField::operator [] (size_t index) const
	{
		return field[index];
	}

	ostream& operator << (ostream& stream, const BaseField& field)
	{
		bool isFileStream = static_cast<bool>(dynamic_cast<ofstream*>(&stream));

		for (int y = static_cast<int>(field.field.size()) - 1; y >= 0; y--)
		{
			for (size_t x = 0; x < field.field[y].size(); x++)
			{
				size_t spacesCount = field.maxTurnDigits - getDigits(field[y][x]) + 1;

				switch (field[y][x])
				{
				case 0:
					stream << 0;

					break;

				case -1:
					stream << '#';

					break;

				case -2:
					stream << '*';

					break;

				default:
					if (isFileStream)
					{
						stream << field[y][x];
					}
					else
					{
						print(field[y][x], stream, field.colors.at(&field[y][x]));
					}

					break;
				}

				stream << string(spacesCount, ' ');
			}

			stream << endl;
		}

		return stream;
	}
}

size_t getDigits(int value)
{
	size_t digits = 0;

	do
	{
		value /= 10;

		digits++;
	} while (value);

	return digits;
}
