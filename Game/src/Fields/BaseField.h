#pragma once

#include <vector>
#include <map>

namespace fields
{
	enum class fieldPointState : uint8_t
	{
		empty,
		filled,
		unaccessed
	};

	class BaseField
	{
	protected:
		std::vector<std::vector<fieldPointState>> field;
		size_t width;
		size_t height;

	protected:
		virtual std::vector<std::vector<fieldPointState>> generateField() const = 0;

	private:
		bool checkPosition(size_t x, size_t y) const;
		
		void findPath(std::vector<std::pair<size_t, size_t>>& possiblePath, std::multimap<size_t, std::vector<std::pair<size_t, size_t>>>& paths, size_t pathSize);

	public:
		BaseField(size_t width, size_t height);

		virtual void generate() final;

		std::vector<std::pair<size_t, size_t>> calculatePossiblePath(size_t currentX, size_t currentY, size_t pathSize);

		std::pair<size_t, size_t> setPlayerPosition();

		std::pair<size_t, size_t> getNextTurnAfterSkip(size_t playerX, size_t playerY) const;

		bool isFieldFull() const;

		std::vector<fieldPointState>& operator [] (size_t index);

		const std::vector<fieldPointState>& operator [] (size_t index) const;

		size_t getWidth() const;

		size_t getHeight() const;

		size_t getAllPositionCount() const;

		virtual ~BaseField() = default;
	};
}
