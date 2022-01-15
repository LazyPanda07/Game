#pragma once

#include "CircleField.h"

namespace fields
{
	class QuadrantField : public CircleField
	{
	protected:
		virtual std::vector<std::vector<int>> generateField() const override;

	public:
		QuadrantField(size_t circleRadius);

		~QuadrantField() = default;
	};
}
