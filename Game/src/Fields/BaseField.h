#pragma once

#include <vector>

namespace fields
{
	class BaseField
	{
	protected:
		std::vector<std::vector<bool>> field;
		size_t width;
		size_t height;

	protected:
		virtual std::vector<std::vector<bool>> generateField() const = 0;

	private:
		bool checkPosition(size_t x, size_t y) const;

	public:
		BaseField(size_t width, size_t height);

		virtual void generate() final;

		std::pair<size_t, size_t> setPlayerPosition();

		std::pair<size_t, size_t> getAvailableNextTurnPosition(size_t playerX, size_t playerY, bool isSkipPreviousTurn) const;

		bool isFieldFull() const;

		std::vector<bool>& operator [] (size_t index);

		const std::vector<bool>& operator [] (size_t index) const;

		size_t getWidth() const;

		size_t getHeight() const;

		size_t getAllPositionCount() const;

		virtual ~BaseField() = default;
	};
}
