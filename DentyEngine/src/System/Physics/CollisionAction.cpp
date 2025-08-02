#include "stdafx.h"

#include "CollisionAction.h"

#include "PhysicsData.h"

namespace DentyEngine
{
	CollisionAction::CollisionAction() :
		btActionInterface()
	{
	}

	// Search for overlapping objects
	// If they overlap, execute the appropriate event function for each
	void CollisionAction::updateAction(btCollisionWorld* collisionWorld, [[maybe_unused]] btScalar deltaTimeStep)
	{
		for (int index = 0; index < collisionWorld->getNumCollisionObjects(); ++index)
		{
			const auto collisionObject = collisionWorld->getCollisionObjectArray()[index];
			const auto internalType = collisionObject->getInternalType();

			using ObjectTypes = btCollisionObject::CollisionObjectTypes;

			// Processing changes depending on internal type.
			switch (internalType)
			{
			case ObjectTypes::CO_RIGID_BODY:
				break;
			case ObjectTypes::CO_GHOST_OBJECT:
			{
				const auto ghostObject = btGhostObject::upcast(collisionObject);
				const auto ghostObjectCollisionData = static_cast<CollisionData*>(ghostObject->getUserPointer());

				for (int overlappingIndex = 0; overlappingIndex < ghostObject->getNumOverlappingObjects(); ++overlappingIndex)
				{
					const auto overlapCollisionObject = ghostObject->getOverlappingObject(overlappingIndex);
					const auto overlapCollisionData = static_cast<CollisionData*>(overlapCollisionObject->getUserPointer());

					// Mimic Unity's event calls
					// If one of them is the trigger, call both OnTriggerXXX-type functions.
					// otherwise, call OnCollisionXXX-type functions.
					{
						ghostObjectCollisionData->AddCollisionData(*overlapCollisionData);
						overlapCollisionData->AddCollisionData(*ghostObjectCollisionData);
					}
				}
			}
			break;
			case ObjectTypes::CO_SOFT_BODY:
				// Nothing to do.
				break;
			default:
				break;
			}
		}
	}

	void CollisionAction::debugDraw([[maybe_unused]] btIDebugDraw* debugDrawer)
	{
		// Nothing to do.
	}
}