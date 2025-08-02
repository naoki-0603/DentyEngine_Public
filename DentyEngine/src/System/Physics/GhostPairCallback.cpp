#include "stdafx.h"

#include "GhostPairCallback.h"

#include "CollisionInfo.h"

#include "System/Editor/EditorSceneManager.h"

#include "System/Events/EventPhysics.h"
#include "System/Scene/Scene.h"

namespace DentyEngine
{
	GhostPairCallback::GhostPairCallback() :
		btGhostPairCallback()
	{
	}

	btBroadphasePair* GhostPairCallback::addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1)
	{
		bool existsProxy0UserPointer = false;
		bool existsProxy1UserPointer = false;

		// Proxy0
		{
			if (proxy0->m_clientObject)
			{
				const auto clientObject = static_cast<btCollisionObject*>(proxy0->m_clientObject);

				existsProxy0UserPointer = (clientObject->getUserPointer() != nullptr);
			}
		}

		// Proxy1
		{
			if (proxy1->m_clientObject)
			{
				const auto clientObject = static_cast<btCollisionObject*>(proxy1->m_clientObject);

				existsProxy1UserPointer = (clientObject->getUserPointer() != nullptr);
			}
		}

		if (existsProxy0UserPointer && existsProxy1UserPointer)
		{
			const auto clientObjectProxy0 = static_cast<btCollisionObject*>(proxy0->m_clientObject);
			const auto clientObjectProxy1 = static_cast<btCollisionObject*>(proxy1->m_clientObject);

			const auto proxy0CollisionInfo = static_cast<CollisionInfo*>(clientObjectProxy0->getUserPointer());
			const auto proxy1CollisionInfo = static_cast<CollisionInfo*>(clientObjectProxy1->getUserPointer());

			const Ref<Scene> activeScene = EditorSceneManager::GetActiveScene();
			if (not activeScene->FindByEntityID(proxy0CollisionInfo->GetOwner()).value()->Enabled())
				return nullptr;

			if (not activeScene->FindByEntityID(proxy1CollisionInfo->GetOwner()).value()->Enabled())
				return nullptr;

			// Proxy0
			{
				CollisionObjectInfo objectInfo = {};
				objectInfo.isTrigger = (clientObjectProxy1->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);

				const bool existsPreviousCollisionObjects = proxy0CollisionInfo->ExistsPrevious(proxy1CollisionInfo->GetOwner());

				// Should be stay state.
				if (existsPreviousCollisionObjects)
				{
					objectInfo.status = CollisionStatus::Stay;
				}
				// Should be enter state.
				else
				{
					objectInfo.status = CollisionStatus::Enter;
				}

				proxy0CollisionInfo->AddCollisionObject(proxy1CollisionInfo->GetOwner(), objectInfo);
			}

			// Proxy1
			{
				CollisionObjectInfo objectInfo = {};
				objectInfo.isTrigger = (clientObjectProxy0->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);

				const bool existsPreviousCollisionObjects = proxy1CollisionInfo->ExistsPrevious(proxy0CollisionInfo->GetOwner());

				// Should be stay state.
				if (existsPreviousCollisionObjects)
				{
					objectInfo.status = CollisionStatus::Stay;
				}
				// Should be enter state.
				else
				{
					objectInfo.status = CollisionStatus::Enter;
				}

				proxy1CollisionInfo->AddCollisionObject(proxy0CollisionInfo->GetOwner(), objectInfo);
			}
		}

		return nullptr;
	}

	void* GhostPairCallback::removeOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1, [[maybe_unused]] btDispatcher* dispatcher)
	{
		bool existsProxy0UserPointer = false;
		bool existsProxy1UserPointer = false;

		// Proxy0
		{
			if (proxy0->m_clientObject)
			{
				const auto clientObject = static_cast<btCollisionObject*>(proxy0->m_clientObject);

				existsProxy0UserPointer = (clientObject->getUserPointer() != nullptr);
			}
		}

		// Proxy1
		{
			if (proxy1->m_clientObject)
			{
				const auto clientObject = static_cast<btCollisionObject*>(proxy1->m_clientObject);

				existsProxy1UserPointer = (clientObject->getUserPointer() != nullptr);
			}
		}

		if (existsProxy0UserPointer && existsProxy1UserPointer)
		{
			const auto clientObjectProxy0 = static_cast<btCollisionObject*>(proxy0->m_clientObject);
			const auto clientObjectProxy1 = static_cast<btCollisionObject*>(proxy1->m_clientObject);

			const auto proxy0CollisionInfo = static_cast<CollisionInfo*>(clientObjectProxy0->getUserPointer());
			const auto proxy1CollisionInfo = static_cast<CollisionInfo*>(clientObjectProxy1->getUserPointer());

			// Proxy0
			{
				CollisionObjectInfo objectInfo = {};
				objectInfo.isTrigger = (clientObjectProxy1->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
				objectInfo.status = CollisionStatus::Exit;

				proxy0CollisionInfo->AddCollisionObject(proxy1CollisionInfo->GetOwner(), objectInfo);
			}

			// Proxy1
			{
				CollisionObjectInfo objectInfo = {};
				objectInfo.isTrigger = (clientObjectProxy0->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
				objectInfo.status = CollisionStatus::Exit;

				proxy1CollisionInfo->AddCollisionObject(proxy0CollisionInfo->GetOwner(), objectInfo);
			}
		}

		return nullptr;
	}

	void GhostPairCallback::removeOverlappingPairsContainingProxy(btBroadphaseProxy* btBroadphaseProxy,
		btDispatcher* btDispatcher)
	{
		btGhostPairCallback::removeOverlappingPairsContainingProxy(btBroadphaseProxy, btDispatcher);
	}

	void GhostPairCallback::CallEnterEvent([[maybe_unused]] CollisionData& colObj0CollisionData, [[maybe_unused]] CollisionData& colObj1CollisionData)
	{
		//// Enter event.
		//if (colObj0CollisionData.collisionType == CollisionType::Trigger || colObj1CollisionData.collisionType == CollisionType::Trigger)
		//{
		//	// col0
		//	if (not colObj0CollisionData.currentFrameIsReceived)
		//	{
		//		EventOnTriggerEnter eventOnTriggerEnter(colObj0CollisionData.ownerID, colObj0CollisionData);
		//		NOTIFY_EVENT(eventOnTriggerEnter);

		//		colObj0CollisionData.currentFrameIsReceived = true;
		//	}

		//	// col1
		//	if (not colObj1CollisionData.currentFrameIsReceived)
		//	{
		//		EventOnTriggerEnter eventOnTriggerEnter(colObj1CollisionData.ownerID, colObj1CollisionData);
		//		NOTIFY_EVENT(eventOnTriggerEnter);

		//		colObj1CollisionData.currentFrameIsReceived = true;
		//	}
		//}
		//else
		//{
		//	// col0
		//	if (not colObj0CollisionData.currentFrameIsReceived)
		//	{
		//		EventOnCollisionEnter eventOnCollisionEnter(colObj0CollisionData.ownerID, colObj0CollisionData);
		//		NOTIFY_EVENT(eventOnCollisionEnter);

		//		colObj0CollisionData.currentFrameIsReceived = true;
		//	}

		//	// col1
		//	if (not colObj1CollisionData.currentFrameIsReceived)
		//	{
		//		EventOnCollisionEnter eventOnCollisionEnter(colObj1CollisionData.ownerID, colObj1CollisionData);
		//		NOTIFY_EVENT(eventOnCollisionEnter);

		//		colObj1CollisionData.currentFrameIsReceived = true;
		//	}
		//}
	}

	void GhostPairCallback::CallExitEvent([[maybe_unused]] CollisionData& colObj0CollisionData, [[maybe_unused]] CollisionData& colObj1CollisionData)
	{
		//// Enter event.
		//if (colObj0CollisionData.collisionType == CollisionType::Trigger || colObj1CollisionData.collisionType == CollisionType::Trigger)
		//{
		//	// col0
		//	if (not colObj0CollisionData.currentFrameIsReceived)
		//	{
		//		EventOnTriggerExit eventOnTriggerExit(colObj0CollisionData.ownerID, colObj0CollisionData);
		//		NOTIFY_EVENT(eventOnTriggerExit);

		//		colObj0CollisionData.currentFrameIsReceived = true;
		//	}

		//	// col1
		//	if (not colObj1CollisionData.currentFrameIsReceived)
		//	{
		//		EventOnTriggerExit eventOnTriggerExit(colObj1CollisionData.ownerID, colObj1CollisionData);
		//		NOTIFY_EVENT(eventOnTriggerExit);

		//		colObj1CollisionData.currentFrameIsReceived = true;
		//	}
		//}
		//else
		//{
		//	// col0
		//	if (not colObj0CollisionData.currentFrameIsReceived)
		//	{
		//		EventOnCollisionExit eventOnCollisionExit(colObj0CollisionData.ownerID, colObj0CollisionData);
		//		NOTIFY_EVENT(eventOnCollisionExit);

		//		colObj0CollisionData.currentFrameIsReceived = true;
		//	}

		//	// col1
		//	if (not colObj1CollisionData.currentFrameIsReceived)
		//	{
		//		EventOnCollisionExit eventOnCollisionExit(colObj1CollisionData.ownerID, colObj1CollisionData);
		//		NOTIFY_EVENT(eventOnCollisionExit);

		//		colObj1CollisionData.currentFrameIsReceived = true;
		//	}
		//}
	}
}
