#pragma once

#include "BaseField.h"

namespace fields
{
	class RectangleField : public BaseField
	{
	protected:
		virtual std::vector<std::vector<int>> generateField() const override;

	public:
		RectangleField(size_t width, size_t height);

		virtual ~RectangleField() = default;
	};
}
