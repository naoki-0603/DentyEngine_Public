#include "stdafx.h"

#include "Raycast.h"

namespace DentyEngine
{
	Ray::Ray(EntityID ownerID) :
		beginPoint(), endPoint(), ownerID(ownerID)
	{
	}

	Ray::Ray(const Vector3& beginPoint, const Vector3& endPoint, EntityID ownerID) :
		beginPoint(beginPoint), endPoint(endPoint), ownerID(ownerID)
	{
	}

	RaycastHit::RaycastHit() :
		hitPosition()
	{
	}
}