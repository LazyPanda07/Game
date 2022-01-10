#pragma once

#include "BaseField.h"

namespace fields
{
	class CircleField : public BaseField
	{
	protected:
		size_t radius;

	protected:
		virtual std::vector<std::vector<fieldPointState>> generateField() const override;

	public:
		CircleField(size_t radius);

		size_t getRadius() const;

		~CircleField() = default;
	};
}
