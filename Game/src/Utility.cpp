#include "Utility.h"

#include <algorithm>

using namespace std;

bool isOpponentPathAround(const pair<size_t, size_t>& playerPosition, const vector<pair<size_t, size_t>>& opponentPossiblePath)
{
	auto left = [](const pair<size_t, size_t>& position) -> pair<size_t, size_t>
	{
		return { position.first - 1, position.second };
	};
	auto top = [](const pair<size_t, size_t>& position) -> pair<size_t, size_t>
	{
		return { position.first, position.second + 1 };
	};
	auto right = [](const pair<size_t, size_t>& position) -> pair<size_t, size_t>
	{
		return { position.first + 1, position.second };
	};
	auto bottom = [](const pair<size_t, size_t>& position) -> pair<size_t, size_t>
	{
		return { position.first, position.second -1 };
	};

	return ranges::any_of(opponentPossiblePath, [&](const pair<size_t, size_t>& position)
		{
			return playerPosition == left(position) ||
				playerPosition == top(position) ||
				playerPosition == right(position) ||
				playerPosition == bottom(position);
		});
}
