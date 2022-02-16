#include "Utility.h"

#include <algorithm>

using namespace std;

tuple<bool, const pair<size_t, size_t>*> isOpponentPathAround(const pair<size_t, size_t>& playerPosition, const vector<pair<size_t, size_t>>& opponentPossiblePath)
{
	const pair<size_t, size_t>* result = nullptr;

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
		return { position.first, position.second - 1 };
	};

	for (const auto& position : opponentPossiblePath)
	{
		if (playerPosition == left(position) ||
			playerPosition == top(position) ||
			playerPosition == right(position) ||
			playerPosition == bottom(position))
		{
			result = &position;

			break;
		}
	}

	return { static_cast<bool>(result), result };
}
