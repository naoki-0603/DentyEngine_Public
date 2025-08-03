#pragma once

#include "System/Core/ID/ComponentID.h"
#include "System/Core/Object.h"

namespace DentyEngine
{
    class GameObject;

    class Component : public Object
    {
    public:
        enum class InheritedType
        {
            Component,
            Behavior,
            PhysicsBehavior,
            MonoBehavior,

            Max
        };
    public:
        Component();
        Component(const Component& source);
        ~Component() override = default;

        virtual void Awake() {}
        virtual void Start() {}
        virtual void OnGui() {}
        virtual void OnPrefabGui() = 0;

    	void OnSerialized() const override;
        void OnDeserialized() override;

        void OnDestroy() override;

        // Call on GameObject::OnDeserialized() function.
        virtual void OnGameObjectDeserialized() {}

        // Call on Scene::OnDeserialized() function.
        virtual void OnSceneDeserialized() {}

        virtual void OnGizmoRender() {}

        [[nodiscard]]
        Ref<Object> MakeCopy() const override;

        //
        // Setter
        //
        void SetParent(const Ref<GameObject>& parent);

        // Use for create from inspector add component.
        void SetComponentID(ComponentID componentID) { _componentID = componentID; }

        void SetEnabled(bool enabled) { _enabled = enabled; }

        //
        // Getter
        //
        [[nodiscard]]
        GameObject* GetParent() const;

        [[nodiscard]]
        const std::vector<std::string>& GetRequiredComponentNames() const { return _requiredComponentNames; }

        [[nodiscard]]
        ComponentID GetComponentID() const { return _componentID; }

        [[nodiscard]]
        InheritedType GetInheritedType() const { return _inheritedType; }

        [[nodiscard]]
        bool IsParentExpired() const { return _parent.expired(); }

    	[[nodiscard]]
        bool Enabled() const { return _enabled; }

    	[[nodiscard]]
        bool MultipleAllowed() const { return _multipleAllowed; }

        [[nodiscard]]
        bool AttachToChildAllowed() const { return _attachToChildAllowed; }
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Object>(this),
                    CEREAL_NVP(_requiredComponentNames),
                    CEREAL_NVP(_componentID),
                    CEREAL_NVP(_inheritedType),
                    CEREAL_NVP(_enabled),
                    CEREAL_NVP(_multipleAllowed),
                    CEREAL_NVP(_attachToChildAllowed)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Object>(this),
                    CEREAL_NVP(_requiredComponentNames),
                    CEREAL_NVP(_componentID),
                    CEREAL_NVP(_inheritedType),
                    CEREAL_NVP(_enabled),
                    CEREAL_NVP(_multipleAllowed),
                    CEREAL_NVP(_attachToChildAllowed)
                );
            }
        }
    protected:
        std::weak_ptr<GameObject> _parent;
        std::vector<std::string> _requiredComponentNames;

        ComponentID _componentID;

        InheritedType _inheritedType;

        bool _enabled;
        bool _multipleAllowed;
        bool _attachToChildAllowed;
    };
};

CEREAL_CLASS_VERSION(DentyEngine::Component, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Component)