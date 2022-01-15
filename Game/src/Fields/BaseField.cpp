#include "BaseField.h"

#include <random>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <fstream>

#include "Utility.h"

using namespace std;

static mt19937_64 random(static_cast<uint32_t>(time(nullptr)));
static constexpr size_t triesToFindEmptyPosition = 100;

namespace fields
{
	bool BaseField::checkPosition(size_t x, size_t y) const
	{
		if (y < field.size())
		{
			if (x < field[y].size() && field[y][x] == fieldPointState::empty)
			{
				return true;
			}
		}

		return false;
	}

	void BaseField::findPath(vector<pair<size_t, size_t>>& possiblePath, multimap<size_t, vector<pair<size_t, size_t>>>& paths, size_t pathSize)
	{
		if (possiblePath.empty())
		{
			return;
		}

		auto recursiveFindPath = [this, &possiblePath, &paths, &pathSize](size_t x, size_t y)
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

				paths.insert(make_pair(possiblePath.size(), possiblePath));

				vector<pair<size_t, size_t>> nextPath = possiblePath;

				this->findPath(nextPath, paths, pathSize);
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
		vector<pair<size_t, size_t>> tem = { make_pair(currentX, currentY) };
		multimap<size_t, vector<pair<size_t, size_t>>> paths;

		this->findPath(tem, paths, pathSize);

		if (paths.empty())
		{
			return {};
		}

		vector<pair<size_t, size_t>> result = paths.rbegin()->second;

		result.erase(result.begin());

		return result;
	}

	void BaseField::fillPosition(size_t x, size_t y, const vector<int64_t>& color)
	{
		field[y][x] = fieldPointState::filled;

		colors[&field[y][x]] = color;
	}

	bool BaseField::isFieldFull() const
	{
		return all_of(field.begin(), field.end(), [](const vector<fieldPointState>& row) { return all_of(row.begin(), row.end(), [](fieldPointState position) { return position == fieldPointState::filled || position == fieldPointState::unaccessed; }); });
	}

	pair<size_t, size_t> BaseField::setPlayerPosition()
	{
		for (size_t i = 0; i < triesToFindEmptyPosition; i++)
		{
			size_t x = random() % width;
			size_t y = random() % height;

			if (this->checkPosition(x, y))
			{
				field[y][x] = fieldPointState::filled;

				return { x, y };
			}
		}

		for (size_t y = 0; y < field.size(); y++)
		{
			for (size_t x = 0; x < field[y].size(); x++)
			{
				if (this->checkPosition(x, y))
				{
					field[y][x] = fieldPointState::filled;

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
				if (field[column][row] == fieldPointState::empty)
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
			result += count_if(i.begin(), i.end(), [](fieldPointState state) { return state == fieldPointState::empty || state == fieldPointState::filled; });
		}

		return result;
	}

	const vector<fieldPointState>& BaseField::operator [] (size_t index) const
	{
		return field[index];
	}

	ostream& operator << (ostream& stream, const BaseField& field)
	{
		bool isFileStream = static_cast<bool>(dynamic_cast<ofstream*>(&stream));

		for (size_t y = 0; y < field.field.size(); y++)
		{
			for (size_t x = 0; x < field.field[y].size(); x++)
			{
				switch (field[y][x])
				{
				case fieldPointState::empty:
					stream << '0';

					break;

				case fieldPointState::filled:
					if (isFileStream)
					{
						stream << '1';
					}
					else
					{
						print('1', stream, field.colors.at(&field[y][x]));
					}
					
					break;

				case fieldPointState::unaccessed:
					stream << '#';

					break;
				}

				stream << ' ';
			}

			stream << endl;
		}

		return stream;
	}
}
