#include "stdafx.h"

#include "DynamicCharacterStatus.h"

namespace DentyEngine
{
	DynamicCharacterStatus::DynamicCharacterStatus() :
		gravityDirection(GravityDirection::Down), steerDirection(SteerDirection::None),
		eulerAngles(), km(), reverseFrameCounter(), handling(), reverse(), onGround()
	{
	}

	void DynamicCharacterStatus::ResetReverseState()
	{
		reverse = false;
		reverseFrameCounter = 0.0f;
	}
}
