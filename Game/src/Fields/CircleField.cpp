#include "CircleField.h"

using namespace std;

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

namespace fields
{
	vector<vector<int>> CircleField::generateField() const
	{
		vector<vector<int>> circle(height, vector<int>(width, -1));
		int64_t centerX = radius;
		int64_t centerY = radius;

		auto checkCirclePosition = [this, &centerX, &centerY](int64_t x, int64_t y)
		{
			static triangle topLeft = { { centerX, centerY }, { 0, centerY }, { centerX, 0 } };
			static triangle topRight = { { centerX, centerY }, { static_cast<int64_t>(width) - 1, centerY }, { centerX, 0 } };
			static triangle bottomLeft = { { centerX, centerY }, { 0, centerY }, { centerX, static_cast<int64_t>(height) - 1 } };
			static triangle bottomRight = { { centerX, centerY }, { static_cast<int64_t>(width) - 1 , centerY }, { centerX, static_cast<int64_t>(height) - 1 } };
			point checkPoint = { x, y };

			return isPointInTriangle(checkPoint, topLeft) ||
				isPointInTriangle(checkPoint, topRight) ||
				isPointInTriangle(checkPoint, bottomLeft) ||
				isPointInTriangle(checkPoint, bottomRight);
		};

		for (int64_t y = 0; y < static_cast<int64_t>(height); y++)
		{
			for (int64_t x = 0; x < static_cast<int64_t>(width); x++)
			{
				if (checkCirclePosition(x, y))
				{
					circle[y][x] = 0;
				}
			}
		}

		return circle;
	}

	CircleField::CircleField(size_t radius) :
		BaseField(radius * 2 + 1, radius * 2 + 1),
		radius(radius)
	{

	}

	size_t CircleField::getRadius() const
	{
		return radius;
	}
}

int64_t checkSign(const point& firstPoint, const point& secondPoint, const point& thirdPoint)
{
	return (firstPoint.x - thirdPoint.x) * (secondPoint.y - thirdPoint.y) - (secondPoint.x - thirdPoint.x) * (firstPoint.y - thirdPoint.y);
}

bool isPointInTriangle(const point& checkPoint, const triangle& triangle)
{
	int64_t v1v2Check = checkSign(checkPoint, triangle.v1, triangle.v2);
	int64_t v2v3Check = checkSign(checkPoint, triangle.v2, triangle.v3);
	int64_t v3v1Check = checkSign(checkPoint, triangle.v3, triangle.v1);
	bool hasNeg = (v1v2Check < 0) || (v2v3Check < 0) || (v3v1Check < 0);
	bool hasPos = (v1v2Check > 0) || (v2v3Check > 0) || (v3v1Check > 0);

	return !(hasNeg && hasPos);
}
