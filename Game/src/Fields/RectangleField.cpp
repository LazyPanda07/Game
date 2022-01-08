#include "RectangleField.h"

using namespace std;

namespace fields
{
	vector<vector<bool>> RectangleField::generateField() const
	{
		return vector<vector<bool>>(height, vector<bool>(width, false));
	}

	RectangleField::RectangleField(size_t width, size_t height) :
		BaseField(width, height)
	{

	}
}
