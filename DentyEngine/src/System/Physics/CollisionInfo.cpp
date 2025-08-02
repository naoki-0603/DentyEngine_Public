#include "stdafx.h"

#include "CollisionInfo.h"

#include "Component/Scripts/ScriptComponent.h"

#include "System/Editor/EditorSceneManager.h"
#include "System/Scene/Scene.h"

namespace DentyEngine
{
	CollisionInfo::CollisionInfo() :
        _collisionObjects(), _previousCollisionObjects(), _owner()
	{
        _owner = 0u;
	}

    void CollisionInfo::Create(EntityID owner)
    {
        _owner = owner;
    }

    void CollisionInfo::Update()
    {
        if (_ownerObject.expired() && _owner != 0u && EditorSceneManager::HasActiveScene())
        {
            const std::optional<Ref<GameObject>> ownerObject = EditorSceneManager::GetActiveScene()->FindByEntityID(_owner);
            if (ownerObject.has_value())
            {
                _ownerObject = ownerObject.value();
            }
        }

        // Check states.
        for (const auto& collisionObject : _collisionObjects)
        {
            switch (collisionObject.second.status)
            {
            case CollisionStatus::Enter:
	            {
					if (collisionObject.second.isTrigger)
					{
                        InvokeOnTriggerEnter(collisionObject.first);
					}
					else
					{
                        InvokeOnCollisionEnter(collisionObject.first);
					}
	            }
                break;
            case CollisionStatus::Stay:
            {
                if (collisionObject.second.isTrigger)
                {
                    InvokeOnTriggerStay(collisionObject.first);
                }
                else
                {
                    InvokeOnCollisionStay(collisionObject.first);
                }
            }
                break;
            case CollisionStatus::Exit:
            {
                if (collisionObject.second.isTrigger)
                {
                    InvokeOnTriggerExit(collisionObject.first);
                }
                else
                {
                    InvokeOnCollisionExit(collisionObject.first);
                }
            }
                break;
            }
        }

        _previousCollisionObjects = _collisionObjects;
        _collisionObjects.clear();
    }

    void CollisionInfo::Reset()
    {
        _previousCollisionObjects.clear();
        _collisionObjects.clear();
    }

    void CollisionInfo::OnGui()
    {
        ImGui::Text("CurrentCollisionObjectSize: %u", _collisionObjects.size());
        ImGui::Text("PreviousCollisionObjectSize: %u", _previousCollisionObjects.size());
        ImGui::Text("Owner: %u", _owner);
    }

    void CollisionInfo::AddCollisionObject(EntityID id, const CollisionObjectInfo& info)
    {
        _collisionObjects.try_emplace(id, info);
    }

    void CollisionInfo::InvokeOnCollisionEnter(EntityID collisionObjectID)
    {
        if (_ownerObject.expired())
            return;

        if (_ownerObject.lock()->HasComponent<ScriptComponent>())
        {
            for (const auto& scriptComponent : _ownerObject.lock()->GetComponentsAsSharedPtr<ScriptComponent>())
            {
	            if (scriptComponent->InvokedOnCreated())
	            {
                    scriptComponent->InvokeOnCollisionEnter(collisionObjectID);
	            }
            }
        }
    }

    void CollisionInfo::InvokeOnCollisionStay(EntityID collisionObjectID)
    {
        if (_ownerObject.expired())
            return;

        if (_ownerObject.lock()->HasComponent<ScriptComponent>())
        {
            for (const auto& scriptComponent : _ownerObject.lock()->GetComponentsAsSharedPtr<ScriptComponent>())
            {
                if (scriptComponent->InvokedOnCreated())
                {
                    scriptComponent->InvokeOnCollisionStay(collisionObjectID);
                }
            }
        }
    }

    void CollisionInfo::InvokeOnCollisionExit(EntityID collisionObjectID)
    {
        if (_ownerObject.expired())
            return;

        if (_ownerObject.lock()->HasComponent<ScriptComponent>())
        {
            for (const auto& scriptComponent : _ownerObject.lock()->GetComponentsAsSharedPtr<ScriptComponent>())
            {
                if (scriptComponent->InvokedOnCreated())
                {
                    scriptComponent->InvokeOnCollisionExit(collisionObjectID);
                }
            }
        }
    }

    void CollisionInfo::InvokeOnTriggerEnter(EntityID collisionObjectID)
    {
        if (_ownerObject.expired())
            return;

        if (_ownerObject.lock()->HasComponent<ScriptComponent>())
        {
            for (const auto& scriptComponent : _ownerObject.lock()->GetComponentsAsSharedPtr<ScriptComponent>())
            {
                if (scriptComponent->InvokedOnCreated())
                {
                    scriptComponent->InvokeOnTriggerEnter(collisionObjectID);
                }
            }
        }
    }

    void CollisionInfo::InvokeOnTriggerStay(EntityID collisionObjectID)
    {
        if (_ownerObject.expired())
            return;

        if (_ownerObject.lock()->HasComponent<ScriptComponent>())
        {
            for (const auto& scriptComponent : _ownerObject.lock()->GetComponentsAsSharedPtr<ScriptComponent>())
            {
                if (scriptComponent->InvokedOnCreated())
                {
                    scriptComponent->InvokeOnTriggerStay(collisionObjectID);
                }
            }
        }
    }

    void CollisionInfo::InvokeOnTriggerExit(EntityID collisionObjectID)
    {
        if (_ownerObject.expired())
            return;

        if (_ownerObject.lock()->HasComponent<ScriptComponent>())
        {
            for (const auto& scriptComponent : _ownerObject.lock()->GetComponentsAsSharedPtr<ScriptComponent>())
            {
                if (scriptComponent->InvokedOnCreated())
                {
                    scriptComponent->InvokeOnTriggerExit(collisionObjectID);
                }
            }
        }
    }

    bool CollisionInfo::ExistsPrevious(EntityID collisionObjectID) const
    {
        return (_previousCollisionObjects.contains(collisionObjectID));
    }
}
