#include "stdafx.h"

#include "Object.h"

namespace DentyEngine
{
	Object::Object(const Object& source) :
		_name(source._name)
	{
		// Recalculate id.
		_uuid.ReCalculate();
	}
};