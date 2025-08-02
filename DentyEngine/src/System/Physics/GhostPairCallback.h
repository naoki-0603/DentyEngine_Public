#pragma once

#include "PhysicsData.h"

namespace DentyEngine
{
	class GhostPairCallback final : public btGhostPairCallback
	{
	public:
		GhostPairCallback();
		~GhostPairCallback() override = default;

		btBroadphasePair* addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) override;
		void* removeOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1, btDispatcher* dispatcher) override;
		void removeOverlappingPairsContainingProxy(btBroadphaseProxy* /*proxy0*/, btDispatcher* /*dispatcher*/) override;

	private:
		void CallEnterEvent(CollisionData& colObj0CollisionData, CollisionData& colObj1CollisionData);
		void CallExitEvent(CollisionData& colObj0CollisionData, CollisionData& colObj1CollisionData);
	};
}