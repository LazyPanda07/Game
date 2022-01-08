#pragma once

#include "RectangleField.h"

namespace fields
{
	class SquareField : public RectangleField
	{
	public:
		SquareField(size_t size);

		~SquareField() = default;
	};
}
