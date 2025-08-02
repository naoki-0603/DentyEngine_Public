#pragma once

#include "System/Core/Object.h"

#include "System/Core/Tag.h"

#include "System/Core/ID/EntityID.h"
#include "System/Core/ID/PrefabID.h"

#include "Component/Transform.h"

#include "System/Events/EventScene.h"

namespace DentyEngine
{
	class EventRuntimeSceneChange;
	class EventRuntimeSceneChanged;
	class Scene;
    class Mesh;
    class StaticMesh;

    class GameObject final : public Object, public std::enable_shared_from_this<GameObject>
    {
    public:
        GameObject();
        GameObject(const GameObject&) = delete;
        ~GameObject() override = default;

        void Create(std::string_view name, const std::string& paramTag, const Scene* paramScene);

        //
        // Create as prefab
        //
        void CreatePrefab(std::string_view name, const std::string& paramTag, const Transform& paramTransform, const Scene* paramScene);

        //
        // Create empty object.
        //
        void CreateEmpty(std::string_view name, const std::string& paramTag, const Scene* paramScene);

    	//
        // Create 3d geometries.
        //

        void CreateBox(const std::string& name, const std::string& paramTag, const Scene* paramScene);
        void CreateSphere(const std::string& name, const std::string& paramTag, const Scene* paramScene);
        void CreateCapsule(const std::string& name, const std::string& paramTag, const Scene* paramScene);
        void CreatePlane(const std::string& name, const std::string& paramTag, const Scene* paramScene);

        //
        // Create with mesh.
        //
        void CreateWithMesh(std::string_view name, const std::string& paramTag, const Scene* paramScene, const Ref<Mesh>& mesh);
        void CreateWithSubMesh(std::string_view name, const std::string& paramTag, const Scene* paramScene, const Ref<Mesh>& mesh, GameObject* parent, uint32_t subMeshIndex);

        //
        // Create 3d lights
        // 

        void CreateDirectionalLight(const std::string& name, const std::string& paramTag, const Scene* paramScene);
        void CreatePointLight(const std::string& name, const std::string& paramTag, const Scene* paramScene);
        void CreateSpotLight(const std::string& name, const std::string& paramTag, const Scene* paramScene);

        //
        // Create ui
        //
        void CreateTextUI(const std::string& name, const std::string& paramTag, const Scene* paramScene);
        void CreateImageUI(const std::string& name, const std::string& paramTag, const Scene* paramScene);
        void CreateGroupUI(const std::string& name, const std::string& paramTag, const Scene* paramScene);

        //
        // Create controllable objects.
        //

        //
        // Vehicle components.
        //

        //
        // Clone function.
        //
        void Clone(Ref<GameObject>& target);

        void Duplicate(Ref<GameObject>& target);

        template <class T>
        Ref<T> AddComponent()
        {
            auto&& component = std::make_shared<T>();

            component->SetParent(shared_from_this());
            component->Awake();

            _uninitializedComponents.emplace_back(component);
            _components.emplace_back(component);

            EventOnComponentAdded eventOnComponentAdded(component);
            NOTIFY_EVENT(eventOnComponentAdded);

            return component;
        }

        // Call from inspector.
        Component* AddComponentOnInspector(const Ref<Component>& component);

        // Add build in component.
        Component* AddBuildInComponent(std::string_view componentName);

        // Add Mono component.
        Component* AddMonoComponent(std::string_view componentName);

        template <class T>
        [[nodiscard]]
        T* GetComponent() const
        {
            for (auto&& component : _components)
            {
                const auto targetComponent = std::dynamic_pointer_cast<T>(component);

                if (!targetComponent) continue;

                return targetComponent.get();
            }

            return nullptr;
        }

        template <class T>
        [[nodiscard]]
        Ref<T> GetComponentAsSharedPtr() const
        {
            for (auto&& component : _components)
            {
                const auto targetComponent = std::dynamic_pointer_cast<T>(component);

                if (!targetComponent) continue;

                return targetComponent;
            }

            return nullptr;
        }

        template <class T>
        [[nodiscard]]
        std::vector<Ref<T>> GetComponentsAsSharedPtr() const
        {
            std::vector<Ref<T>> components = {};
            for (auto&& component : _components)
            {
                const auto targetComponent = std::dynamic_pointer_cast<T>(component);

                if (!targetComponent) continue;

                components.emplace_back(targetComponent);
            }

            return components;
        }

        [[nodiscard]]
        const std::vector<Ref<Component>>& GetComponents() const { return _components; }

        // HACK Optimize
        template <class T>
        [[nodiscard]] bool HasComponent() const
        {
            for (auto&& component : _components)
            {
                const auto targetComponent = std::dynamic_pointer_cast<T>(component);

                if (targetComponent)
                {
                    return true;
                }
            }

            return false;
        }

        [[nodiscard]]
        bool HasComponent(std::string_view componentName) const;

        // HACK Optimize
        template <class T>
        void RemoveComponent()
        {
            const bool hasComponent = HasComponent<T>();
            DENTY_ASSERT(hasComponent, "Does not have this component. (in RemoveComponent)");

            const auto& component = GetComponentAsSharedPtr<T>();
            _components.erase(component);

            EventOnComponentRemoved eventOnComponentRemoved(component);
            NOTIFY_EVENT(eventOnComponentRemoved);
        }

        void OnUpdate(const float deltaTime);
        void OnLateUpdate(float deltaTime);
        void OnFixedUpdate();
        void OnGui();

        void OnGizmoRender();

        void OnBeforeSave() override;

        void OnSerialized() const override;
        void OnDeserialized() override;

        void OnEvent(Event* e) override;

        void OnDestroy() override;

        void ReserveSetParent(GameObject* parent);

        void AddChild(GameObject* child, bool onSceneDeserialized = false);
        void ReserveAddChild(GameObject* child);

        void ReserveRemoveChild(GameObject* child);
        void RemoveChild(GameObject* child);

        //
        // Event functions.
        //

        // Call on EditorContentBrowser::PrefabAssetDragAndDropTargetCallback function.
        void OnPrefabricated(const FilePath&);
        void OnPrefabChanged();

        //
        // Setter
        //
        void SetParent(GameObject* parent);
        void SetComponent(const Ref<Component>& component);
        void SetComponents(const std::vector<Ref<Component>>& components) { _components = components; }
        void SetPrefabFilePath(const FilePath& filePath) { _prefabFilePath = filePath; }
        void SetEntityID(EntityID entityID) { _entityId = entityID; }

        void SetEnabled(bool enabled);
        void SetStatic(bool staticFlag) { _static = staticFlag; }
        void SetPrefab(bool prefab) { _prefab = prefab; }

        //
        // Find functions.
        //
        [[nodiscard]]
        GameObject* FindChildByName(std::string_view name) const;

        [[nodiscard]]
    	GameObject* FindChildByEntityID(EntityID id) const;

    	// Obtain is top parent is enabled.
        // Note that the function is called recursively, so if it is too deep, the process will be slow.
        [[nodiscard]] bool IsTopParentEnabled() const;

        //
        // Clear function.
        //

        // Call on Scene::OnSceneActivated function.
        void ClearChildren() { _children.clear(); }

        //
        // Getter
        //
        [[nodiscard]]
        EntityID GetParentEntityID() const { return _parentEntityID; }

        [[nodiscard]]
        const std::vector<EntityID>& GetChildEntityIDs() const { return _childEntityIDs; }

        [[nodiscard]]
        GameObject* GetParent() const { return _parent; }

        [[nodiscard]]
        const std::vector<GameObject*>& GetChildren() const { return _children; }

        [[nodiscard]]
        const std::vector<Ref<Component>>& GetUninitializedComponents() const { return _uninitializedComponents; }

        [[nodiscard]]
        EntityID GetEntityID() const { return _entityId; }

        [[nodiscard]]
        const FilePath& GetPrefabFilePath() const { return _prefabFilePath; }

        [[nodiscard]] 
        bool HasParent() const { return (_parent != nullptr); }

        [[nodiscard]] 
        bool HasChild() const { return !_children.empty(); }

        [[nodiscard]] 
        bool Enabled() const { return _enabled; }

        [[nodiscard]] 
        bool Static() const { return _static; }

        [[nodiscard]] 
        bool Prefab() const { return _prefab; }
    public:
        Tag tag;
        Transform transform;

        const Scene* scene;
    private:
        void CreateCanvas(const Scene* paramScene);

        //
        // Event functions.
        //
        void OnSceneActivated(EventOnSceneActivated* e) const;
        void OnSceneDeActivated(EventOnSceneDeActivated* e) const;

        void OnRuntimeSceneChange(EventRuntimeSceneChange* e);
        void OnRuntimeSceneChanged(EventRuntimeSceneChanged* e);

        void TagChanged();


        // Check child is duplicate.
        [[nodiscard]]
    	bool IsChildDuplicate(EntityID id);
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Object>(this),
                    CEREAL_NVP(tag),
                    CEREAL_NVP(transform),
                    CEREAL_NVP(_components),
                    CEREAL_NVP(_parentEntityID),
                    CEREAL_NVP(_childEntityIDs),
                    CEREAL_NVP(_entityId),
                    CEREAL_NVP(_prefabFilePath),
                    CEREAL_NVP(_enabled),
                    CEREAL_NVP(_static),
                    CEREAL_NVP(_prefab)
                );
            }
            
            OnSerialized();
        }

        template <class Archive>
        void load(Archive& archive, uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Object>(this),
                    CEREAL_NVP(tag),
                    CEREAL_NVP(transform),
                    CEREAL_NVP(_components),
                    CEREAL_NVP(_parentEntityID),
                    CEREAL_NVP(_childEntityIDs),
                    CEREAL_NVP(_entityId),
                    CEREAL_NVP(_prefabFilePath),
                    CEREAL_NVP(_enabled),
                    CEREAL_NVP(_static),
                    CEREAL_NVP(_prefab)
                );
            }

            OnDeserialized();
        }
    private:
        // Components that are not executing start functions are stored.
        std::vector<Ref<Component>> _uninitializedComponents;
        std::vector<Ref<Component>> _components;
        Ref<Component> _reservedRemoveComponent;

        // For serialize
        std::vector<EntityID> _childEntityIDs;
        EntityID _parentEntityID;

        // For reserve.
        GameObject* _reservedParent;
        std::vector<GameObject*> _reservedAddChildren;
        std::vector<GameObject*> _reservedRemoveChildren;

        GameObject* _parent;
        std::vector<GameObject*> _children;

        // GameObject id.
        EntityID _entityId;

        // Prefab file path.
        // _prefab is true, contain prefab.
        FilePath _prefabFilePath;

        bool _enabled;

        // If valid, do not calculate position.
        bool _static;

        // If prefabricated, it would be true.
        bool _prefab;
    };
};

CEREAL_CLASS_VERSION(DentyEngine::GameObject, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::GameObject)