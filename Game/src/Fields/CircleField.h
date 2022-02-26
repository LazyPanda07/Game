#pragma once

#include "BaseField.h"

namespace fields
{
	namespace circle_utility
	{
		struct point
		{
			int64_t x;
			int64_t y;
		};

		struct triangle
		{
			point v1;
			point v2;
			point v3;
		};

		int64_t checkSign(const point& firstPoint, const point& secondPoint, const point& thirdPoint);

		bool isPointInTriangle(const point& checkPoint, const triangle& triangle);
	}

	class CircleField : public BaseField
	{
	protected:
		size_t radius;

	protected:
		virtual std::vector<std::vector<int>> generateField() const override;

	public:
		CircleField(size_t radius);

		size_t getRadius() const;

		~CircleField() = default;
	};
}
