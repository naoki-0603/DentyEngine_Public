#include "stdafx.h"

#include "PhysicsData.h"

#include <ranges>

#include "../Events/EventPhysics.h"

namespace DentyEngine
{
	void CollisionData::Update()
	{
		// The hit collision data is stored.
		for (const auto& hitCollisionData : currentCollisionDataUMap | std::views::values)
		{
			if (hitCollisionData.collisionType == CollisionType::Trigger || collisionType == CollisionType::Trigger)
			{
				EventOnTriggerStay eventOnTriggerStay(hitCollisionData.ownerID, hitCollisionData);
				NOTIFY_EVENT(eventOnTriggerStay);
			}
			else
			{
				EventOnCollisionStay eventOnCollisionStay(hitCollisionData.ownerID, hitCollisionData);
				NOTIFY_EVENT(eventOnCollisionStay);
			}
		}

		currentCollisionDataUMap = { };
		objectIDs = {};
	}

	void CollisionData::AddCollisionData(CollisionData& collisionData)
	{
		currentCollisionDataUMap.emplace(collisionData.ownerID, collisionData);
	}
}