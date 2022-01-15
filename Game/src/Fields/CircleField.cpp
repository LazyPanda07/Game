#include "CircleField.h"

#include <iostream>

using namespace std;

namespace fields
{
	vector<vector<int>> CircleField::generateField() const
	{
		vector<vector<int>> circle(height, vector<int>(width, -1));
		int64_t centerX = radius;
		int64_t centerY = radius;
		int64_t squareRadius = static_cast<int64_t>(pow(radius, 2));

		for (int64_t y = 0; y < static_cast<int64_t>(height); y++)
		{
			for (int64_t x = 0; x < static_cast<int64_t>(width); x++)
			{
				if ((pow(x - centerX, 2) + pow(y - centerY, 2)) <= squareRadius)
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
