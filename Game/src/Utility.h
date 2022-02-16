#pragma once

#include <cstdint>
#include <ostream>
#include <vector>
#include <format>

template<typename T>
void print(const T& symbol, std::ostream& stream, const std::vector<int64_t>& color)
{
	stream << std::format("\033[38;2;{};{};{}m{}\033[0m", color[0], color[1], color[2], symbol);
}

std::tuple<bool, const std::pair<size_t, size_t>*> isOpponentPathAround(const std::pair<size_t, size_t>& playerPosition, const std::vector<std::pair<size_t, size_t>>& opponentPossiblePath);
