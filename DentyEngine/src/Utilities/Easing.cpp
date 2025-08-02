#include "stdafx.h"

#include "Easing.h"

namespace DentyEngine
{
	Easing::Easing()
	{
	}

	float Easing::EaseOutCirc(float x)
	{
		if (x >= 1.0f)
		{
			return 1.0f;
		}
		else if (x < 0.0f)
		{
			return 0.0f;
		}

		return std::sqrtf(1.0f - std::powf(x - 1.0f, 2.0f));
	}
}
