#include "RectangleField.h"

using namespace std;

namespace fields
{
	vector<vector<fieldPointState>> RectangleField::generateField() const
	{
		return vector<vector<fieldPointState>>(height, vector<fieldPointState>(width, fieldPointState::empty));
	}

	RectangleField::RectangleField(size_t width, size_t height) :
		BaseField(width, height)
	{

	}
}
