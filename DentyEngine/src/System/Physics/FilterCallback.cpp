#include "stdafx.h"

#include "FilterCallback.h"

namespace DentyEngine
{
	FilterCallback::FilterCallback() :
		btOverlapFilterCallback()
	{
	}

	bool FilterCallback::needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
	{
		bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
		collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

		return collides;
	}
}
