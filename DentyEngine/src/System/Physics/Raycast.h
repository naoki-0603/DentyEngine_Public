#pragma once

#include "System/Core/ID/EntityID.h"

namespace DentyEngine
{
	struct Ray
	{
		Ray(EntityID ownerID);
		Ray(const Vector3& beginPoint, const Vector3& endPoint, EntityID ownerID);

		Vector3 beginPoint;
		Vector3 endPoint;

		// Ray owner.
		EntityID ownerID;
	};

	struct RaycastHit
	{
		RaycastHit();

		Vector3 hitPosition;
	};

	static constexpr uint32_t RAYCAST_RESULT_MAX = 32u;
}