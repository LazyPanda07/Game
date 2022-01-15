#include "RectangleField.h"

using namespace std;

namespace fields
{
	vector<vector<int>> RectangleField::generateField() const
	{
		return vector<vector<int>>(height, vector<int>(width, 0));
	}

	RectangleField::RectangleField(size_t width, size_t height) :
		BaseField(width, height)
	{

	}
}
