#pragma once

#include "System/Core/ID/EntityID.h"

namespace DentyEngine
{
    enum class CollisionStatus : int32_t
    {
	    None = -1,
        Enter = 0,
        Stay,
        Exit,

        Max
    };

    struct CollisionObjectInfo
    {
    public:
        CollisionObjectInfo() : status(CollisionStatus::None), isTrigger() {}

    public:
        CollisionStatus status;

        bool isTrigger;
    };

    class GameObject;

    class CollisionInfo final
    {
    public:
        CollisionInfo();
        ~CollisionInfo() = default;

        void Create(EntityID owner);

        void Update();

        void Reset();

        // For debug.
        void OnGui();

        void AddCollisionObject(EntityID id, const CollisionObjectInfo& info);

        // C# script functions.
        void InvokeOnCollisionEnter(EntityID collisionObjectID);
        void InvokeOnCollisionStay(EntityID collisionObjectID);
        void InvokeOnCollisionExit(EntityID collisionObjectID);

        void InvokeOnTriggerEnter(EntityID collisionObjectID);
        void InvokeOnTriggerStay(EntityID collisionObjectID);
        void InvokeOnTriggerExit(EntityID collisionObjectID);

        [[nodiscard]]
        bool ExistsPrevious(EntityID collisionObjectID) const;

        [[nodiscard]]
        EntityID GetOwner() const { return _owner; }
    private:
        std::unordered_map<EntityID, CollisionObjectInfo> _collisionObjects;
        std::unordered_map<EntityID, CollisionObjectInfo> _previousCollisionObjects;

        EntityID _owner;

        std::weak_ptr<GameObject> _ownerObject;
    };
}