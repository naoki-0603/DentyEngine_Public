#include "stdafx.h"

#include "GameObject.h"

#include "Graphics/Renderer/Texture2D.h"

#include "Component/Behavior.h"
#include "Component/MonoBehavior.h"
#include "Component/Controller/DefaultCameraController.h"

#include "Component/Graphics/Renderer/MeshRenderer.h"

#include "Component/Graphics/Renderer/DirectionalLight.h"
#include "Component/Graphics/Renderer/EffectComponent.h"
#include "Component/Graphics/Renderer/PointLight.h"
#include "Component/Graphics/Renderer/SkinnedMeshRenderer.h"
#include "Component/Graphics/Renderer/SpotLight.h"

#include "Component/Physics/DynamicCharacterController.h"
#include "Component/Physics/Colliders/Collider.h"
#include "Component/Physics/Colliders/BoxCollider.h"
#include "Component/Physics/Colliders/SphereCollider.h"
#include "Component/Physics/Colliders/CapsuleCollider.h"
#include "Component/Physics/Colliders/MeshCollider.h"

#include "Component/Physics/RigidBody.h"

#include "Component/Physics/Vehicle/VehicleComponent.h"

#include "Component/Scripts/ScriptComponent.h"

#include "Component/Graphics/Renderer/GameCamera.h"

#include "Component2D/UI/UIComponent.h"

#include "UI/Items/Image.h"

#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/SkinnedMesh.h"
#include "Graphics/Mesh/StaticMesh.h"

#include "Graphics/PostEffect/ShadowMap/ShadowMap.h"

#include "System/Events/EventScene.h"
#include "System/Events/EventSystem.h"

#include "System/Graphics/Renderer/LightManager.h"
#include "System/Graphics/Renderer/RenderManager.h"

#include "System/Scene/Scene.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/MeshLibrary.h"
#include "System/Resource/Library/PrefabLibrary.h"
#include "UI/Items/Group.h"

#include "Utilities/ImGuiUtilities.h"

#include "Utilities/StringUtilities.h"

#pragma warning(disable: 26859)
#pragma warning(disable: 6031)

namespace DentyEngine
{
    GameObject::GameObject() :
        tag(Tag::TAGS.at(0)), transform(), scene(),
        _uninitializedComponents(), _components(), _reservedRemoveComponent(), _childEntityIDs(), _parentEntityID(),
        _reservedParent(), _reservedAddChildren(), _reservedRemoveChildren(), _parent(), _children(),
        _entityId(), _prefabFilePath(), _enabled(true), _static(), _prefab()
    {
        // Initialize with 0 because it is generated when initializing.
        _parentEntityID = 0u;
    }

    //
    // Create GameObjects
    // 

    void GameObject::Create(std::string_view name, const std::string& paramTag, const Scene* paramScene)
    {
        _name = name;

        this->tag = paramTag;
        this->scene = paramScene;

        // Set transform parent.
        transform.SetParent(shared_from_this());
    }

    void GameObject::CreatePrefab(std::string_view name, const std::string& paramTag, const Transform& paramTransform, const Scene* paramScene)
    {
        Create(name, paramTag, paramScene);

        transform = paramTransform;
        transform.SetParent(shared_from_this());

        EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
        NOTIFY_EVENT(eventOnCreatedGameObject);
    }

    void GameObject::CreateEmpty(std::string_view name, const std::string& paramTag, const Scene* paramScene)
    {
        Create(name, paramTag, paramScene);

        EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
        NOTIFY_EVENT(eventOnCreatedGameObject);
    }

    //
    // Create 3D Geometries 
    //

    void GameObject::CreateBox([[maybe_unused]] const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        // Find box mesh.
        std::optional<Ref<StaticMesh>> boxMesh;
        {
            const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
            boxMesh = meshLibrary->Find<StaticMesh>(MeshLibrary::BOX_FILE_PATH);

            DENTY_ASSERT(boxMesh.has_value(), "Box mesh not found! (in GameObject::CreateBox)");
        }

        // Create game objects.
        {
            CreateWithMesh(boxMesh.value()->GetName(), paramTag, paramScene, boxMesh.value());

            // Sub meshes.
            for (uint32_t subMeshIndex = 0u; subMeshIndex < boxMesh.value()->GetSubMeshCount(); ++subMeshIndex)
            {
                Ref<Mesh> subMesh = boxMesh.value()->GetSubMeshes().at(subMeshIndex);

                const auto subMeshGameObject = std::make_shared<GameObject>();
                subMeshGameObject->CreateWithSubMesh(subMesh->GetName(), paramTag, paramScene, subMesh, this, subMeshIndex);
                subMeshGameObject->AddComponent<BoxCollider>();
            }
        }
    }

    void GameObject::CreateSphere([[maybe_unused]] const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        // Find sphere mesh.
        std::optional<Ref<StaticMesh>> sphereMesh;
    	{
            const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
            sphereMesh = meshLibrary->Find<StaticMesh>(MeshLibrary::SPHERE_FILE_PATH);

            DENTY_ASSERT(sphereMesh.has_value(), "Sphere mesh not found! (in GameObject::CreateSphere)");
        }

        // Create game objects.
        {
            CreateWithMesh(sphereMesh.value()->GetName(), paramTag, paramScene, sphereMesh.value());

            // Sub meshes.
            for (uint32_t subMeshIndex = 0u; subMeshIndex < sphereMesh.value()->GetSubMeshCount(); ++subMeshIndex)
            {
                Ref<Mesh> subMesh = sphereMesh.value()->GetSubMeshes().at(subMeshIndex);

                const auto subMeshGameObject = std::make_shared<GameObject>();
                subMeshGameObject->CreateWithSubMesh(subMesh->GetName(), paramTag, paramScene, subMesh, this, subMeshIndex);
                subMeshGameObject->AddComponent<SphereCollider>();
            }
        }
    }

    void GameObject::CreateCapsule([[maybe_unused]] const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        // Find capsule mesh.
        std::optional<Ref<StaticMesh>> capsuleMesh;
        {
            const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
            capsuleMesh = meshLibrary->Find<StaticMesh>(MeshLibrary::CAPSULE_FILE_PATH);

            DENTY_ASSERT(capsuleMesh.has_value(), "Capsule mesh not found! (in GameObject::CreateSphere)");
        }

        // Create game objects.
        {
            CreateWithMesh(capsuleMesh.value()->GetName(), paramTag, paramScene, capsuleMesh.value());

            // Sub meshes.
            for (uint32_t subMeshIndex = 0u; subMeshIndex < capsuleMesh.value()->GetSubMeshCount(); ++subMeshIndex)
            {
                Ref<Mesh> subMesh = capsuleMesh.value()->GetSubMeshes().at(subMeshIndex);

                const auto subMeshGameObject = std::make_shared<GameObject>();
                subMeshGameObject->CreateWithSubMesh(subMesh->GetName(), paramTag, paramScene, subMesh, this, subMeshIndex);
                subMeshGameObject->AddComponent<CapsuleCollider>();
            }
        }
    }

    void GameObject::CreatePlane([[maybe_unused]] const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        // Find plane mesh.
        std::optional<Ref<StaticMesh>> planeMesh;
        {
            const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
            planeMesh = meshLibrary->Find<StaticMesh>(MeshLibrary::PLANE_FILE_PATH);

            DENTY_ASSERT(planeMesh.has_value(), "Plane mesh not found! (in GameObject::CreateSphere)");
        }

        // Create game objects.
        {
            CreateWithMesh(planeMesh.value()->GetName(), paramTag, paramScene, planeMesh.value());

            // Sub meshes.
            for (uint32_t subMeshIndex = 0u; subMeshIndex < planeMesh.value()->GetSubMeshCount(); ++subMeshIndex)
            {
                Ref<Mesh> subMesh = planeMesh.value()->GetSubMeshes().at(subMeshIndex);

                const auto subMeshGameObject = std::make_shared<GameObject>();
                subMeshGameObject->CreateWithSubMesh(subMesh->GetName(), paramTag, paramScene, subMesh, this, subMeshIndex);
                subMeshGameObject->AddComponent<MeshCollider>();
            }
        }
    }

    void GameObject::CreateWithMesh(std::string_view name, const std::string& paramTag, const Scene* paramScene,
        const Ref<Mesh>& mesh)
    {
        Create(name, paramTag, paramScene);

        if (mesh->GetMeshType() == std::string("StaticMesh"))
        {
            const auto meshRenderer = AddComponent<MeshRenderer>();

        	const auto staticMesh = std::make_shared<StaticMesh>();
            mesh->Clone(staticMesh);

        	meshRenderer->SetMesh(staticMesh);
        }
        else if (mesh->GetMeshType() == std::string("SkinnedMesh"))
        {
            const auto skinnedMeshRenderer = AddComponent<SkinnedMeshRenderer>();

        	const auto skinnedMesh = std::make_shared<SkinnedMesh>();
            mesh->Clone(skinnedMesh);

            skinnedMeshRenderer->SetMesh(skinnedMesh);
        }

        EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
        NOTIFY_EVENT(eventOnCreatedGameObject);
    }

    void GameObject::CreateWithSubMesh(std::string_view name, const std::string& paramTag, const Scene* paramScene,
                                       const Ref<Mesh>& mesh, GameObject* parent, uint32_t subMeshIndex)
    {
        Create(name, paramTag, paramScene);

        if (mesh->GetMeshType() == std::string("StaticMesh"))
        {
            const auto meshRenderer = AddComponent<MeshRenderer>();
            meshRenderer->SetSubMeshIndex(subMeshIndex);

            const auto staticMesh = std::make_shared<StaticMesh>();
            mesh->Clone(staticMesh);

            meshRenderer->SetMesh(staticMesh);
        }
        else if (mesh->GetMeshType() == std::string("SkinnedMesh"))
        {
            const auto skinnedMeshRenderer = AddComponent<SkinnedMeshRenderer>();
            skinnedMeshRenderer->SetSubMeshIndex(subMeshIndex);

            const auto skinnedMesh = std::make_shared<SkinnedMesh>();
            mesh->Clone(skinnedMesh);

            skinnedMeshRenderer->SetMesh(skinnedMesh);
        }

        SetParent(parent);
        parent->AddChild(this);

        EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
        NOTIFY_EVENT(eventOnCreatedGameObject);
    }

    //
    // Create Lights
    //

    void GameObject::CreateDirectionalLight(const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        Create(name, paramTag, paramScene);

        AddComponent<DirectionalLight>();

        // Notify game object created event.
        EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
        NOTIFY_EVENT(eventOnCreatedGameObject);
    }

    void GameObject::CreatePointLight(const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        Create(name, paramTag, paramScene);

        AddComponent<PointLight>();

        // Notify game object created event.
        EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
        NOTIFY_EVENT(eventOnCreatedGameObject);
    }

    void GameObject::CreateSpotLight(const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        Create(name, paramTag, paramScene);

        AddComponent<SpotLight>();

        // Notify game object created event.
        EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
        NOTIFY_EVENT(eventOnCreatedGameObject);
    }

    void GameObject::CreateTextUI(const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        // Create canvas if doesn't exists.
        Ref<GameObject> canvas = nullptr;
        {
            if (paramScene->ExistsCanvas())
            {
                canvas = paramScene->FindByTag("Canvas").value();
            }
            else
            {
                canvas = std::make_shared<GameObject>();
                canvas->CreateCanvas(paramScene);
            }
        }

        // Create text ui object.
        {
            Create(name, paramTag, paramScene);

            const auto component = AddComponent<UIComponent>();

            const auto text = std::make_shared<UIText>();
            text->Create();

            component->Setup(text);

            // Make child-parent relation ships.
            {
                canvas->AddChild(this);
                SetParent(canvas.get());
            }

            // Notify game object created event.
            EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
            NOTIFY_EVENT(eventOnCreatedGameObject);
        }
    }

    void GameObject::CreateImageUI(const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        // Create canvas if doesn't exists.
        Ref<GameObject> canvas = nullptr;
        {
            if (paramScene->ExistsCanvas())
            {
                canvas = paramScene->FindByTag("Canvas").value();
            }
            else
            {
                canvas = std::make_shared<GameObject>();
                canvas->CreateCanvas(paramScene);
            }
        }

        // Create text ui object.
        {
            Create(name, paramTag, paramScene);

            const auto component = AddComponent<UIComponent>();

            const auto image = std::make_shared<UIImage>();
            image->Create();

            component->Setup(image);

            // Make child-parent relation ships.
            {
                canvas->AddChild(this);
                SetParent(canvas.get());
            }

            // Notify game object created event.
            EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
            NOTIFY_EVENT(eventOnCreatedGameObject);
        }
    }

    void GameObject::CreateGroupUI(const std::string& name, const std::string& paramTag, const Scene* paramScene)
    {
        // Create canvas if doesn't exists.
        Ref<GameObject> canvas = nullptr;
        {
            if (paramScene->ExistsCanvas())
            {
                canvas = paramScene->FindByTag("Canvas").value();
            }
            else
            {
                canvas = std::make_shared<GameObject>();
                canvas->CreateCanvas(paramScene);
            }
        }

        // Create text ui object.
        {
            Create(name, paramTag, paramScene);

            const auto component = AddComponent<UIComponent>();

            const auto group = std::make_shared<UIGroup>();
            group->Create();

            component->Setup(group);

            // Make child-parent relation ships.
            {
                canvas->AddChild(this);
                SetParent(canvas.get());
            }

            // Notify game object created event.
            EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
            NOTIFY_EVENT(eventOnCreatedGameObject);
        }
    }

    //
    // Create controllable objects.
    //

    void GameObject::Clone(Ref<GameObject>& target)
    {
        // Base class member values.
        {
            target->_name = _name;
            target->_uuid = _uuid;
        }

        // This class member values.
        {
            target->tag = tag;
            target->transform = transform;

            target->_parentEntityID = _parentEntityID;
            target->_childEntityIDs = _childEntityIDs;

            target->_entityId = _entityId;
            target->_enabled = _enabled;
            target->_prefab = _prefab;
            target->_prefabFilePath = _prefabFilePath;
            target->_static = _static;

            // Make copy components.
            for (auto&& sourceComponent : _components)
            {
                target->_components.emplace_back(std::static_pointer_cast<Component>(sourceComponent->MakeCopy()));
            }
        }
    }

    void GameObject::Duplicate(Ref<GameObject>& target)
    {
        // Object
        {
            target->_name = _name;
        }

        // This class
        {
            target->tag = tag;
            target->transform = transform;

            target->_prefabFilePath = _prefabFilePath;

            target->_enabled = _enabled;
            target->_static = _static;
            target->_prefab = _prefab;

            // Make copy of components.
            for (auto&& sourceComponent : _components)
            {
                target->SetComponent(std::static_pointer_cast<Component>(sourceComponent->MakeCopy()));
            }

            target->scene = scene;
        }

        // Parent-child relations
        {
	        if (_parentEntityID != 0u)
	        {
                GetParent()->AddChild(target.get());
                target->SetParent(GetParent());
	        }
        }
    }

    //
    // Component functions.
    //

    Component* GameObject::AddComponentOnInspector(const Ref<Component>& component)
    {
        component->SetParent(shared_from_this());
        component->Awake();

        _components.emplace_back(component);
        _uninitializedComponents.emplace_back(component);

        return component.get();
    }

    Component* GameObject::AddBuildInComponent(std::string_view componentName)
    {
        const auto component = ComponentGenerator::FindBuildInComponent(componentName);

        if (not component) return nullptr;
        if (not component->MultipleAllowed())
        {
            if (HasComponent(componentName))
            {
                DENTY_ERR_CONSOLE_LOG("Already has %s component!", component->GetName().c_str());

            	return nullptr;
            }
        }

        if (not component->AttachToChildAllowed())
        {
	        if (HasParent())
	        {
                DENTY_ERR_CONSOLE_LOG("Can be attach to parent only!");

                return nullptr;
            }
        }

        // Check is required components exists.
        for (const auto& requiredComponentName : component->GetRequiredComponentNames())
        {
            if (HasComponent(requiredComponentName)) continue;

            DENTY_ERR_CONSOLE_LOG("%s component is required!", requiredComponentName.c_str());

            return nullptr;
        }

        return AddComponentOnInspector(component);
    }

    bool GameObject::HasComponent(std::string_view componentName) const
    {
        for (const auto& component : _components)
        {
            if (component->GetName() == componentName) return true;
            if (StringUtils::StringContains(component->GetName(), componentName)) return true;
        }

        return false;
    }

    void GameObject::OnUpdate(const float deltaTime)
    {
        // Initialize added component.
        for (auto&& component : _uninitializedComponents)
        {
            component->Start();
        }
        _uninitializedComponents.clear();

        // Remove 
        if (_reservedRemoveComponent)
        {
            _reservedRemoveComponent->OnDestroy();

            _components.erase(std::ranges::find(_components, _reservedRemoveComponent));

            DENTY_SET_INFO_COLOR
            DENTY_INFO_LOG << _reservedRemoveComponent->GetName() << " component was successfully deleted from component list!";
        }
        _reservedRemoveComponent = nullptr;

        // Add child
        if (not _reservedAddChildren.empty())
        {
            for (const auto& reservedChild : _reservedAddChildren)
            {
                AddChild(reservedChild);
            }

            _reservedAddChildren.clear();
        }

        // Remove child
        if (not _reservedRemoveChildren.empty())
        {
            for (const auto& reservedChild : _reservedRemoveChildren)
            {
                RemoveChild(reservedChild);
            }

            _reservedRemoveChildren.clear();
        }

        // Set parent
        if (_reservedParent)
        {
            SetParent(_reservedParent);

            _reservedParent = nullptr;
        }

        if (not _static)
        {
            transform.OnUpdate(deltaTime);
        }

        // Update components
        for (auto&& component : _components)
        {
            if (not component->Enabled()) continue;

            switch (component->GetInheritedType())
            {
            case Component::InheritedType::Component:
                // Nothing to do.
                break;
            case Component::InheritedType::Behavior:
                std::static_pointer_cast<Behavior>(component)->OnUpdate(deltaTime);
                break;
            case Component::InheritedType::PhysicsBehavior:
            {
                const auto& physicsBehavior = std::static_pointer_cast<PhysicsBehavior>(component);
                physicsBehavior->OnUpdate(deltaTime);
            }
            case Component::InheritedType::MonoBehavior:
                // Nothing to do.
                break;
            default:
                DENTY_ASSERT(false, "Unknown inherited type.");
                break;
            }
        }
    }

    void GameObject::OnLateUpdate([[maybe_unused]] float deltaTime)
    {

    }

    // Call update on fixed time.
    void GameObject::OnFixedUpdate()
    {
        for (const auto& component : _components)
        {
            if (not component->Enabled()) continue;

            switch (component->GetInheritedType())
            {
            case Component::InheritedType::Component:
                // Nothing to do.
                break;
            case Component::InheritedType::Behavior:
                std::static_pointer_cast<Behavior>(component)->OnFixedUpdate();
                break;
            case Component::InheritedType::PhysicsBehavior:
                std::static_pointer_cast<PhysicsBehavior>(component)->OnFixedUpdate();
                break;
            case Component::InheritedType::MonoBehavior:
                std::static_pointer_cast<MonoBehavior>(component)->OnFixedUpdate();
                break;
            default:
                DENTY_ASSERT(false, "Unknown inherited type.");
                break;
            }
        }
    }

    // GUI
    void GameObject::OnGui()
    {

        bool gameObjectEnabled = _enabled; 
        if (ImGui::Checkbox("##GameObjectEnabled", &gameObjectEnabled))
        {
            SetEnabled(gameObjectEnabled);
        }

        ImGui::SameLine();

        String::InputTextGui("##GameObjectName", &_name);

        ImGui::SameLine();
        ImGui::Checkbox("Static##GameObject", &_static);

        ImGui::Text("Tag");
        ImGui::SameLine();

        {
            // Check tag was changed.
            if (not GuiUtils::ComboBoxGui("##GameObjectTag", Tag::TAGS.data(), static_cast<int32_t>(Tag::TAGS.size()), tag.tag))
            {
                TagChanged();
            }

            ImGui::SameLine();

            if (HasChild())
            {
                if (ImGui::Button("ApplyChildren##Tag"))
                {
                    for (auto&& child : GetChildren())
                    {
                        child->tag = tag;

                        child->TagChanged();
                    }
                }

                ImGui::SameLine();

                GuiUtils::HelpMarker("Apply parent tag to children.");
            }
        }
        

        ImGui::SameLine();

        ImGui::Separator();

        if (ImGui::TreeNodeEx("Transform##GameObject", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
        {
            transform.OnGui();

            ImGui::Separator();

            ImGui::TreePop();
        }

        for (auto&& component : _components)
        {
            bool enabled = component->Enabled();

            ImGui::PushID(static_cast<int32_t>(component->GetComponentID()));

            ImGui::Checkbox("##ComponentEnabled", &enabled);

            ImGui::PopID();

            component->SetEnabled(enabled);

            ImGui::SameLine();
            if (ImGui::TreeNodeEx(component->GetName().c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen))
            {
                // Remove component button.
                {
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionWidth() - 48.0f);
                    if (ImGui::Button("Remove"))
                    {
                        _reservedRemoveComponent = component;
                    }
                }

                component->OnGui();

                ImGui::Separator();
                ImGui::TreePop();
            }
        }
    }

    void GameObject::OnGizmoRender()
    {
        for (const auto& component : _components)
        {
            component->OnGizmoRender();
        }
    }

    void GameObject::OnBeforeSave()
    {
        for (auto&& component : _components)
        {
            component->OnBeforeSave();
        }
    }

    void GameObject::OnSerialized() const
    {

    }

    void GameObject::OnDeserialized()
    {
        transform.SetParent(shared_from_this());
        transform.OnGameObjectDeserialized();

        for (auto&& component : _components)
        {
            // Set Parent
            component->SetParent(shared_from_this());

            component->OnGameObjectDeserialized();
        }
    }

    // Called from Scene::OnEvent function.

    void GameObject::OnEvent(Event* e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<EventOnSceneActivated>(BIND_EVENT_FUNCTION(OnSceneActivated));
        dispatcher.Dispatch<EventOnSceneDeActivated>(BIND_EVENT_FUNCTION(OnSceneDeActivated));

        dispatcher.Dispatch<EventRuntimeSceneChange>(BIND_EVENT_FUNCTION(OnRuntimeSceneChange));
        dispatcher.Dispatch<EventRuntimeSceneChanged>(BIND_EVENT_FUNCTION(OnRuntimeSceneChanged));

        for (const auto& component : _components)
        {
            component->OnEvent(e);
        }
    }

    // Called from Scene::OnDestroy function.

    void GameObject::OnDestroy()
    {
        Object::OnDestroy();

        for (const auto& component : _components)
        {
            component->OnDestroy();
        }

        EventOnGameObjectDestroyed eventOnGameObjectDestroyed(_entityId);
        NOTIFY_EVENT(eventOnGameObjectDestroyed);
    }

    void GameObject::SetParent(GameObject* parent)
    {
        _parent = parent;

        if (not _parent)
        {
            _parentEntityID = 0u;
        }

        if (_parent)
        {
            _parentEntityID = parent->GetEntityID();
        }
    }

    void GameObject::SetComponent(const Ref<Component>& component)
    {
        component->SetParent(shared_from_this());
        component->Awake();

        _uninitializedComponents.emplace_back(component);

        _components.emplace_back(component);
    }

    void GameObject::ReserveSetParent(GameObject* parent)
    {
        _reservedParent = parent;
    }

    void GameObject::AddChild(GameObject* child, bool onSceneDeserialized)
    {
        if (onSceneDeserialized)
        {
            _children.emplace_back(child);
        }

        // If duplicate, return
        if (IsChildDuplicate(child->GetEntityID()))
            return;

        _children.emplace_back(child);
        _childEntityIDs.emplace_back(child->GetEntityID());
    }

    void GameObject::ReserveAddChild(GameObject* child)
    {
        _reservedAddChildren.emplace_back(child);
    }

    void GameObject::ReserveRemoveChild(GameObject* child)
    {
        _reservedRemoveChildren.emplace_back(child);
    }

    void GameObject::RemoveChild(GameObject* child)
    {
        if (bool erased = Utils::EraseFromVectorContainer(_children, child))
        {
            Utils::EraseFromVectorContainer(_childEntityIDs, child->GetEntityID());

            DENTY_SET_INFO_COLOR
            DENTY_INFO_LOG << "Successes remove game object child. (Name: " << child->GetName().c_str() << ", ID: " << child->GetEntityID() << ")";
        }
    }

    void GameObject::OnPrefabricated(const FilePath& prefabFilePath)
    {
        SetPrefabFilePath(prefabFilePath);
        SetPrefab(true);

        DENTY_SET_INFO_COLOR
        DENTY_INFO_LOG << "GameObject " << _name << " was prefabricated! (id: " << _entityId << " )";
    }

    void GameObject::OnPrefabChanged()
    {
        const PrefabLibrary* prefabLibrary = LibraryManager::GetInstance().GetPrefabLibrary();

    	const std::optional<Ref<DentyEngine::Prefab>> prefab = prefabLibrary->Find(_prefabFilePath);
        if (prefab.has_value())
        {
            // Currently only support tag.
            tag = prefab.value()->GetSpecification().parent.tag;
        }
    }

    void GameObject::SetEnabled(bool enabled)
    {
        if (enabled)
        {
            EventGameObjectEnabled eventGameObjectEnabled(shared_from_this());
            NOTIFY_EVENT(eventGameObjectEnabled);
        }
        else
        {
            EventGameObjectDisabled eventGameObjectDisabled(shared_from_this());
            NOTIFY_EVENT(eventGameObjectDisabled);
        }

        _enabled = enabled;
    }

    //
    // Find functions.
    //

    GameObject* GameObject::FindChildByName(std::string_view name) const
    {
        for (auto&& child : _children)
        {
            if (child->GetName() == name)
            {
                return child;
            }
        }

        return nullptr;
    }

    GameObject* GameObject::FindChildByEntityID(EntityID id) const
    {
        for (auto&& child : _children)
        {
            if (child->GetEntityID() == id)
            {
                return child;
            }
        }

        return nullptr;
    }

    bool GameObject::IsTopParentEnabled() const
    {
        if (_parent)
        {
            return _parent->IsTopParentEnabled();
        }

        return _enabled;
    }

    void GameObject::CreateCanvas(const Scene* paramScene)
    {
        Create("Canvas", "Canvas", paramScene);

        EventOnCreatedGameObject eventOnCreatedGameObject(shared_from_this());
        NOTIFY_EVENT(eventOnCreatedGameObject);
    }

    //
    // Static functions.
    //
    void GameObject::OnSceneActivated([[maybe_unused]] EventOnSceneActivated* e) const
    {
        // GameCamera
        if (HasComponent<GameCamera>())
        {
            if (tag() == "MainCamera")
            {
                const auto gameCamera = GetComponentAsSharedPtr<GameCamera>();

                GameCamera::SetMainCamera(gameCamera);   
            }
        }

        // Light
        if (HasComponent<Light>())
        {
            const auto light = GetComponentAsSharedPtr<Light>();

            if (light->GetType() == Light::LightType::Directional)
            {
                ShadowMap::GetInstance()->SetDirectionalLight(
                    std::static_pointer_cast<DirectionalLight>(light)
                );
            }

            LightManager::GetInstance().AddLight(light);
        }

        // Renderer
        if (HasComponent<Renderer>())
        {
            const auto renderer = GetComponentAsSharedPtr<Renderer>();
            renderer->OnSceneActivated();
        }

        // Effect
        if (HasComponent<EffectComponent>())
        {
            const auto effect = GetComponentAsSharedPtr<EffectComponent>();
            effect->OnSceneActivated();
        }

        // Collider
        if (HasComponent<Collider>())
        {
        	const auto collider = GetComponentAsSharedPtr<Collider>();
            collider->OnSceneActivated();
        }

        // Rigid body
        if (HasComponent<RigidBody>())
        {
            const auto rigidBody = GetComponentAsSharedPtr<RigidBody>();
            rigidBody->OnSceneActivated();
        }

        if (HasComponent<DefaultCameraController>())
        {
            const auto defaultCameraController = GetComponentAsSharedPtr<DefaultCameraController>();
            defaultCameraController->OnSceneActivated();
        }

        if (HasComponent<DynamicCharacterController>())
        {
            const auto controller = GetComponentAsSharedPtr<DynamicCharacterController>();

            controller->Restore();
        }

        // Has script component
        if (HasComponent<ScriptComponent>())
        {
            const auto scriptComponent = GetComponentAsSharedPtr<ScriptComponent>();
            scriptComponent->OnSceneActivated();
        }

        if (HasComponent<VehicleComponent>())
        {
            const auto vehicleComponent = GetComponentAsSharedPtr<VehicleComponent>();
            vehicleComponent->OnSceneActivated();
        }

        if (HasComponent<UIComponent>())
        {
            const auto uiComponent = GetComponentAsSharedPtr<UIComponent>();
            uiComponent->OnSceneActivated();
        }
    }

    void GameObject::OnSceneDeActivated([[maybe_unused]] EventOnSceneDeActivated* e) const
    {
        // GameCamera
        if (HasComponent<GameCamera>())
        {
            if (tag() == std::string("MainCamera"))
            {
                GameCamera::ResetMainCamera(); 
            }
        }

        // Light
        if (HasComponent<Light>())
        {
            using LightType = Light::LightType;

            const auto light = GetComponentAsSharedPtr<Light>();

            if (light->GetType() == Light::LightType::Directional)
            {
                ShadowMap::GetInstance()->SetDirectionalLight(
                    nullptr
                );
            }

            LightManager::GetInstance().RemoveLight(light);
        }

        // Renderer
        if (HasComponent<Renderer>())
        {
            const auto renderer = GetComponentAsSharedPtr<Renderer>();

            RenderManager::GetInstance().RemoveRenderer(renderer);
        }

        // Effect
        if (HasComponent<EffectComponent>())
        {
            const auto effect = GetComponentAsSharedPtr<EffectComponent>();
            effect->OnSceneDeActivated();
        }

        // Collider
        if (HasComponent<Collider>())
        {
            const auto collider = GetComponentAsSharedPtr<Collider>();
            collider->OnSceneDeActivated();
        }

        // Rigid body
        if (HasComponent<RigidBody>())
        {
            const auto rigidBody = GetComponentAsSharedPtr<RigidBody>();
            rigidBody->OnSceneDeActivated();
        }

        if (HasComponent<DefaultCameraController>())
        {
            const auto defaultCameraController = GetComponentAsSharedPtr<DefaultCameraController>();
            defaultCameraController->OnSceneDeActivated();
        }

        if (HasComponent<DynamicCharacterController>())
        {
            
        }

        if (HasComponent<VehicleComponent>())
        {
            const auto vehicleComponent = GetComponentAsSharedPtr<VehicleComponent>();
            vehicleComponent->OnSceneDeActivated();
        }

        if (HasComponent<UIComponent>())
        {
            const auto uiComponent = GetComponentAsSharedPtr<UIComponent>();
            uiComponent->OnSceneDeActivated();
        }
    }

    void GameObject::OnRuntimeSceneChange(EventRuntimeSceneChange* e)
    {
        /*if (HasComponent<ScriptComponent>())
        {
            const auto scriptComponent = GetComponentAsSharedPtr<ScriptComponent>();
            scriptComponent->OnRuntimeSceneChange();
        }*/
    }

    void GameObject::OnRuntimeSceneChanged(EventRuntimeSceneChanged* e)
    {
        if (HasComponent<Collider>())
        {
            const auto collider = GetComponentAsSharedPtr<Collider>();
            collider->OnRuntimeSceneChanged();
        }

        if (HasComponent<RigidBody>())
        {
            const auto rigidBody = GetComponentAsSharedPtr<RigidBody>();
            rigidBody->OnRuntimeSceneChanged();
        }

        if (HasComponent<VehicleComponent>())
        {
            const auto vehicleComponent = GetComponentAsSharedPtr<VehicleComponent>();
            vehicleComponent->OnRuntimeSceneChanged();
        }

        /*if (HasComponent<ScriptComponent>())
        {
            const auto scriptComponent = GetComponentAsSharedPtr<ScriptComponent>();
            scriptComponent->OnRuntimeSceneChanged(e->GetSceneName());
        }*/

        if (HasComponent<UIComponent>())
        {
            const auto uiComponent = GetComponentAsSharedPtr<UIComponent>();
            uiComponent->OnRuntimeSceneChanged();
        }
    }

    void GameObject::TagChanged()
    {
        if (HasComponent<Collider>())
        {
            GetComponentAsSharedPtr<Collider>()->OnGameObjectTagChanged();
        }

        if (HasComponent<RigidBody>())
        {
            GetComponentAsSharedPtr<RigidBody>()->OnGameObjectTagChanged();
        }

        if (HasComponent<VehicleComponent>())
        {
            GetComponentAsSharedPtr<VehicleComponent>()->OnGameObjectTagChanged();
        }
    }

   

    bool GameObject::IsChildDuplicate(EntityID id)
    {
        for (auto&& entityId : _childEntityIDs)
        {
            if (id == entityId) return true;
        }

        return false;
    }
};

#pragma warning(default: 26859)
#pragma warning(default: 6031)
