#include "QuadrantField.h"

#include <iostream>

using namespace std;

namespace fields
{
	vector<vector<int>> QuadrantField::generateField() const
	{
		vector<vector<int>> circle = CircleField::generateField();
		QuadrantField& self = const_cast<QuadrantField&>(*this);
		
		self.width = radius + 1;
		self.height = radius + 1;
		
		vector<vector<int>> quadrant(height, vector<int>(width, 0));

		for (size_t y = 0; y <= radius; y++)
		{
			for (size_t x = 0; x <= radius; x++)
			{
				quadrant[y][x] = circle[y][x];
			}
		}

		return quadrant;
	}

	QuadrantField::QuadrantField(size_t circleRadius) :
		CircleField(circleRadius)
	{

	}
}
