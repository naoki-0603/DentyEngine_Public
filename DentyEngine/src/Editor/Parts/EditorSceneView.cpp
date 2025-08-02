#include "stdafx.h"

#include "EditorSceneView.h"

#include "System/Core/Window.h"

#include "Component/Graphics/Renderer/SkinnedMeshRenderer.h"
#include "Editor/Graphics/Rendering/EditorCamera.h"

#include "Graphics/Buffer/ColorBuffer.h"
#include "Graphics/Buffer/SceneBuffer.h"

#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/SceneTree.h"

#include "System/Events/EventRender.h"
#include "System/Events/EventEditor.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Layer/EditorLayer.h"

#include "System/Physics/PhysicsEngine.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/PrefabLibrary.h"

#include "System/Scene/Scene.h"

#include "Scene/GameObject.h"
#include "System/Core/Tag.h"
#include "System/Core/ID/PrefabID.h"

#include "System/Editor/EditorSceneManager.h"

namespace DentyEngine
{
    Scope<EditorCamera> EditorSceneView::_editorCamera = nullptr;

    Vector2I EditorSceneView::_viewportSize = {};
    Vector2I EditorSceneView::_viewportPosition = {};

    EditorSceneView::EditorSceneView(ID3D11Device* device) :
        EditorView(), _manipulateGizmo(), _selectedGameObject(), _previewGameObject(),
        _previewChildren(), _dragAndDropReceiveType(), _isHoveredWindow(), _isDragAndDrop()
    {
        Camera::Perspective perspective = {};
        perspective.nearZ = 0.1f;
        perspective.farZ = 5000.0f;
        perspective.up = Vector3::Up();
        perspective.fovAngleY = 45.0f;
        perspective.distance = 1.0f;

        const Vector2 size = Window::GetInfo().GetSize<Vector2>();
        perspective.aspectRatio = size.x / size.y;

        _editorCamera = std::make_unique<EditorCamera>(device, perspective);

        _manipulateGizmo = std::make_unique<ManipulateGizmo>();
        _manipulateGizmo->Initialize();
    }

    EditorSceneView::~EditorSceneView()
    {
        _manipulateGizmo->Finalize();
    }

    void EditorSceneView::OnUpdate(float deltaTime)
    {
        // Editor camera.
        {
            const bool isEditorCameraMoving = _editorCamera->IsMoving();
            if (isEditorCameraMoving || _isHoveredWindow)
            {
                _editorCamera->UpdateInput(deltaTime);
            }

            _editorCamera->OnUpdate();
        }

        // Gizmos.
        {
            _manipulateGizmo->Update(_editorCamera->GetView(), _editorCamera->GetProjection());
        }
    }

    void EditorSceneView::OnEvent(Event* e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<EventSelectedGameObject>(BIND_EVENT_FUNCTION(OnSelectedGameObject));

        if (_editorCamera)
        {
            _editorCamera->OnEvent(e);
        }

        _manipulateGizmo->OnEvent(e);
    }

    //
    // Control, Render gui.
    //
    void EditorSceneView::OnGui(Ref<Scene> activeScene)
    {
        // Get d3d11 devices.
        ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();
        ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

        EventRequiredSceneViewColorData e = {};
        NOTIFY_EVENT(e);

        constexpr ImGuiWindowFlags windowFlags = {};

        // Set style
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, WINDOW_PADDING_Y));

        static bool open = true;
        if (ImGui::Begin("SceneView##Editor", &open, windowFlags))
        {
            EditorView::OnGui(activeScene);

            /*if (ApplicationLayer::IsPlay())
            {
                ImGui::PopStyleVar(2);

                ImGui::End();

                return;
            }*/

            // Get window data.
            const Vector2 windowPosition = Vector2::ToVector2(ImGui::GetWindowPos());
            const Vector2 windowSize = Vector2::ToVector2(ImGui::GetWindowSize());

            // Store is window hovered.
            _isHoveredWindow = ImGui::IsWindowHovered();

            // Store static variables.
            _viewportSize = windowSize.ToInt();
            _viewportPosition = windowPosition.ToInt();

            // Check is view size changed.
            if (IsViewSizeChanged(_previousSize, _currentSize))
            {
                if (!_currentSize.IsContainMinus())
                {
                    // OnViewSizeChanged
                    e.GetSceneBuffer()->OnResize(device, _currentSize);

                    // Event
                    EventOnSceneViewSizeChanged onViewSizeChanged(_currentSize);
                    NOTIFY_EVENT(onViewSizeChanged);
                }
            }

            // GetColorBuffer.
            const ImTextureID colorMap = *e.GetSceneBuffer()->GetColorBuffer()->GetResource();
            ImGui::Image(colorMap, ImVec2(_currentSize.x, _currentSize.y));

            {
                // Get there is entity exists.
                if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && not ImGuizmo::IsOver())
                {
                    PickEntity(device, context, e.GetSceneBuffer().get(), activeScene.get());
                }

                SetFocusPosition();
            }

            // Drag and drop receive functions.
            {
                ReceivePrefabItem(activeScene.get(), windowPosition, windowSize);
                ReceiveMeshItem(activeScene.get(), windowPosition, windowSize);
                ReceiveSkyboxItem(activeScene.get());
            }

            _manipulateGizmo->Render(windowPosition, windowSize);

        }
        ImGui::End();

        ImGui::PopStyleVar(2);

        // Call editor camera ongui.
        _editorCamera->OnGui();

        _isDragAndDrop = false;
    }

    void EditorSceneView::OnSelectedGameObject(EventSelectedGameObject* e)
    {
        _selectedGameObject = e->GetGameObject();
    }

    void EditorSceneView::RenderGizmos()
    {
        if (EditorSceneManager::HasActiveScene())
        {
            const auto activeScene = EditorSceneManager::GetActiveScene();

            for (const auto& gameObject : activeScene->GetGameObjects())
            {
                if (!gameObject->Enabled())
                    continue;

                gameObject->OnGizmoRender();
            }
        }
    }

    void EditorSceneView::SetFocusPosition()
    {
        if (_selectedGameObject)
        {
            Vector3 focusPosition = _selectedGameObject->transform.position;

            /*if (_selectedGameObject->HasComponent<MeshRenderer>())
            {
                const auto meshRenderer = _selectedGameObject->GetComponentAsSharedPtr<MeshRenderer>();
                if (meshRenderer->HasMesh())
                {
                    const Ref<Mesh> mesh = meshRenderer->GetMesh();
                    if (mesh->ParentNode())
                    {
                        focusPosition = mesh->GetAllVerticesBoundingBox().GetCenter();
                    }
                    else
                    {
                        const Ref<Mesh> root = mesh->GetRoot();
                        const Ref<Mesh> subMesh = root->GetSubMeshes().at(meshRenderer->GetSubMeshIndex());

                        focusPosition = focusPosition - subMesh->GetBoundingBox().GetHalfSize();
                    }
                }
            }
            else if (_selectedGameObject->HasComponent<SkinnedMeshRenderer>())
            {
                const auto skinnedMeshRenderer = _selectedGameObject->GetComponentAsSharedPtr<SkinnedMeshRenderer>();
            }*/

            _editorCamera->SetFocusPosition(focusPosition);
        }
    }

    void EditorSceneView::PickEntity(ID3D11Device* device, ID3D11DeviceContext* context,
                                     const SceneBuffer* sceneBuffer, Scene* activeScene)
    {
        const auto mousePosition = Vector2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
        const auto windowPosition = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + OFFSET_Y);
        const auto windowSize = Vector2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

        if (mousePosition.x - windowPosition.x < 0.0f ||
            mousePosition.y - windowPosition.y < 0.0f)
        {
            return;
        }

        const Vector2 fixPosition = Vector2(windowPosition - mousePosition).Absolute();

        if (fixPosition.x > windowSize.x ||
            fixPosition.y > windowSize.y)
        {
            return;
        }

        const uint32_t id = sceneBuffer->ReadPixel(device, context, *sceneBuffer->GetEntityIDBuffer()->GetResource(),
            DXGI_FORMAT_R32_UINT, sceneBuffer->GetEntityIDBuffer()->GetSize(), fixPosition
        );

        if (id <= 0) return;

        const auto gameObject = activeScene->FindByEntityID(id);
        if (!gameObject.has_value())
        {
            DENTY_SET_ERR_COLOR
            DENTY_ERR_LOG << "GameObject " << id << " not found!";

            return;
        }

        EventSelectedGameObject e(gameObject.value().get());
        NOTIFY_EVENT(e);
    }

    void EditorSceneView::UpdatePreviewGameObject(const Vector2& windowPosition, const Vector2& windowSize)
    {
        // Get mouse position.
        const Vector2I mousePosition = Input::GetMousePosition();

        // Raycast
        RaycastHit hit = { };
        {
            Vector3 previewPosition = Vector3::Zero();

            // Convert to world.
            const Vector3 beginPositionInWorld = Vector3::ScreenToWorld(Vector3(mousePosition, 0.0f), windowPosition.x, windowPosition.y,
                windowSize.x, windowSize.y, 0.0f, 1.0f,
                &_editorCamera->GetView(), &_editorCamera->GetProjection()
            );

            const Vector3 endPositionInWorld = Vector3::ScreenToWorld(Vector3(mousePosition, 1.0f), windowPosition.x, windowPosition.y,
                windowSize.x, windowSize.y, 0.0f, 1.0f,
                &_editorCamera->GetView(), &_editorCamera->GetProjection()
            );

            const Ray ray = { beginPositionInWorld, endPositionInWorld, _previewGameObject->GetEntityID() };

            if (PhysicsEngine::GetInstance()->Raycast(ray, Color::RED, hit))
            {
                previewPosition = hit.hitPosition;
            }
            else
            {
                previewPosition = Vector3::ScreenToWorld(Vector3(mousePosition, 0.97f), windowPosition.x, windowPosition.y,
                    windowSize.x, windowSize.y, 0.0f, 1.0f,
                    &_editorCamera->GetView(), &_editorCamera->GetProjection()
                );
            }

            // Update position.
            _previewGameObject->transform.localPosition = previewPosition;
        }
    }

    void EditorSceneView::ReceivePrefabItem([[maybe_unused]] Scene* activeScene, const Vector2& windowPosition, const Vector2& windowSize)
    {
        // Drag and drop target before delivery.
        {
            const DragAndDropSpecification specification =
            {
                "PrefabItem", sizeof(PrefabID),
                nullptr, DragAndDropDataType::Prefab,
                DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(PrefabItemDragAndDropCallbackTargetBeforeDelivery),
                DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(PrefabItemDragAndDropCallbackTargetInDestructor),
                ImGuiDragDropFlags_AcceptBeforeDelivery
            };
            DragAndDropTarget dragAndDropTarget(specification);
        }

        // Drag and drop target.
        {
            const DragAndDropSpecification specification =
            {
                "PrefabItem", sizeof(PrefabID),
                nullptr, DragAndDropDataType::Prefab,
                DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(PrefabItemDragAndDropCallbackTarget),
                DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
            };
            DragAndDropTarget dragAndDropTarget(specification);
        }

        if (_previewGameObject)
        {
            UpdatePreviewGameObject(windowPosition, windowSize);
        }
    }

    void EditorSceneView::ReceiveMeshItem([[maybe_unused]] Scene* activeScene, const Vector2& windowPosition, const Vector2& windowSize)
    {
        // Drag and drop target before delivery.
        {
            const DragAndDropSpecification specification =
            {
                "MeshItem", sizeof(Utils::DragAndDropMeshData),
                nullptr, DragAndDropDataType::Mesh,
                DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(MeshItemDragAndDropCallbackTargetBeforeDelivery),
                DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(MeshItemDragAndDropCallbackTargetInDestructor),
                ImGuiDragDropFlags_AcceptBeforeDelivery
            };
            DragAndDropTarget dragAndDropTarget(specification);
        }

        // Drag and drop target.
        {
            const DragAndDropSpecification specification =
            {
                "MeshItem", sizeof(Utils::DragAndDropMeshData),
                nullptr, DragAndDropDataType::Mesh,
                DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(MeshItemDragAndDropCallbackTarget),
                DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
            };
            DragAndDropTarget dragAndDropTarget(specification);
        }

        if (_previewGameObject)
        {
            UpdatePreviewGameObject(windowPosition, windowSize);
        }
    }

    void EditorSceneView::ReceiveSkyboxItem([[maybe_unused]] Scene* activeScene)
    {
        // Drag and drop target.
        {
            const DragAndDropSpecification specification =
            {
                "SkyboxItem", sizeof(Utils::DragAndDropSkyboxData),
                nullptr, DragAndDropDataType::Skybox,
                DENTY_BIND_DRAG_AND_DROP_CALLBACK_FUNCTION(SkyboxItemDragAndDropCallbackTarget),
                DENTY_BIND_PASS_DRAG_AND_DROP_CALLBACK_FUNCTION()
            };
            DragAndDropTarget dragAndDropTarget(specification);
        }
    }

    void EditorSceneView::PrefabItemDragAndDropCallbackTargetBeforeDelivery(void* data, DragAndDropDataType dragAndDropDataType, [[maybe_unused]] const Ref<void>& userData)
    {
        DENTY_ASSERT(dragAndDropDataType == DragAndDropDataType::Prefab, "Drag and drop data type must be prefab!");

        _dragAndDropReceiveType = DragAndDropReceiveType::Prefab;

        if (_previewGameObject)
            return;

        const PrefabLibrary* prefabLibrary = LibraryManager::GetInstance().GetPrefabLibrary();

        const auto activeScene = EditorSceneManager::GetActiveScene();

        const auto id = *static_cast<const PrefabID*>(data);
        const auto prefab = prefabLibrary->Find(id);

        if (!prefab.has_value())
        {
            DENTY_SET_ERR_COLOR
            DENTY_ERR_LOG << "Prefab " << id << " not found!";

            return;
        }

        const auto& gameObjects = prefab.value()->CreateGameObjects(activeScene.get(), Vector3::Zero());

        _previewGameObject = gameObjects.at(0);
    }

    void EditorSceneView::PrefabItemDragAndDropCallbackTarget(void* data, DragAndDropDataType dragAndDropDataType, [[maybe_unused]] const Ref<void>& userData)
    {
        DENTY_ASSERT(dragAndDropDataType == DragAndDropDataType::Prefab, "Drag and drop data type must be prefab!");

        const auto id = *static_cast<const PrefabID*>(data);

        if (id)
            _previewGameObject = nullptr;

        _isDragAndDrop = true;
    }

    void EditorSceneView::PrefabItemDragAndDropCallbackTargetInDestructor([[maybe_unused]] void* data, DragAndDropDataType dragAndDropDataType, [[maybe_unused]] const Ref<void>& userData)
    {
        DENTY_ASSERT(dragAndDropDataType == DragAndDropDataType::Prefab, "Drag and drop data type must be prefab!");

        if (not _previewGameObject)
            return;

        const auto activeScene = EditorSceneManager::GetActiveScene();

        // Remove children from scene.
        {
            for (auto&& subMesh : _previewChildren)
            {
                activeScene->ReserveRemoveGameObject(subMesh);
            }
        }

        {
            activeScene->ReserveRemoveGameObject(_previewGameObject);
        }

        _previewGameObject = nullptr;
        _previewChildren.clear();

        _dragAndDropReceiveType = DragAndDropReceiveType::None;
    }

    void EditorSceneView::MeshItemDragAndDropCallbackTargetBeforeDelivery(void* data, DragAndDropDataType dragAndDropDataType, [[maybe_unused]] const Ref<void>& userData)
    {
        DENTY_ASSERT(dragAndDropDataType == DragAndDropDataType::Mesh, "Drag and drop data type must be mesh!");

        _dragAndDropReceiveType = DragAndDropReceiveType::Mesh;

        if (_previewGameObject)
            return;

        [[maybe_unused]] const MeshLibrary* meshLibrary = LibraryManager::GetInstance().GetMeshLibrary();
        const auto activeScene = EditorSceneManager::GetActiveScene();

        const auto dragAndDropMeshData = (static_cast<Utils::DragAndDropMeshData*>(data));
        const auto mesh = dragAndDropMeshData->Get();

        if (!mesh)
        {
            DENTY_SET_ERR_COLOR
            DENTY_ERR_LOG << "Mesh " << mesh->GetFilePathToSerialized() << " not found!";

            return;
        }

        const auto gameObject = std::make_shared<GameObject>();

        gameObject->CreateWithMesh(mesh->GetName(), Tag::TAGS.at(0), activeScene.get(), mesh);

        // Sub meshes.
        for (uint32_t subMeshIndex = 0u; subMeshIndex < mesh->GetSubMeshCount(); ++subMeshIndex)
        {
            const auto child = std::make_shared<GameObject>();
            const Ref<Mesh> subMesh = mesh->GetSubMeshes().at(subMeshIndex);

            child->CreateWithSubMesh(subMesh->GetName(), Tag::TAGS.at(0), activeScene.get(), subMesh, gameObject.get(), subMeshIndex);

            _previewChildren.emplace_back(child);
        }

        _previewGameObject = gameObject;

        _isDragAndDrop = true;
    }

    void EditorSceneView::MeshItemDragAndDropCallbackTarget([[maybe_unused]] void* data, DragAndDropDataType dragAndDropDataType, [[maybe_unused]] const Ref<void>& userData)
    {
        DENTY_ASSERT(dragAndDropDataType == DragAndDropDataType::Mesh, "Drag and drop data type must be mesh!");

        const auto activeScene = EditorSceneManager::GetActiveScene();

        // Set nullptr.
        _previewGameObject = nullptr;

        _previewChildren.clear();

        _isDragAndDrop = true;
    }

    void EditorSceneView::MeshItemDragAndDropCallbackTargetInDestructor([[maybe_unused]] void* data, DragAndDropDataType dragAndDropDataType, [[maybe_unused]] const Ref<void>& userData)
    {
        DENTY_ASSERT(dragAndDropDataType == DragAndDropDataType::Mesh, "Drag and drop data type must be mesh!");

        if (not _previewGameObject)
            return;

        const auto activeScene = EditorSceneManager::GetActiveScene();

        // Remove children from scene.
        {
            for (auto&& subMesh : _previewChildren)
            {
                activeScene->ReserveRemoveGameObject(subMesh);
            }
        }

        {
            activeScene->ReserveRemoveGameObject(_previewGameObject);
        }

        _previewGameObject = nullptr;
        _previewChildren.clear();

        _dragAndDropReceiveType = DragAndDropReceiveType::None;
    }

    void EditorSceneView::SkyboxItemDragAndDropCallbackTarget(void* data, DragAndDropDataType dragAndDropDataType, [[maybe_unused]] const Ref<void>& userData)
    {
        DENTY_ASSERT(dragAndDropDataType == DragAndDropDataType::Skybox, "Drag and drop data type must be skybox!");

        const auto skyboxData = *static_cast<Utils::DragAndDropSkyboxData*>(data);

        if (skyboxData.Exists())
        {
            const auto skybox = skyboxData.Get();

            OnSkyboxItemReceived(skybox);
        }
    }

    void EditorSceneView::OnSkyboxItemReceived(Ref<Skybox> skybox)
    {
        const auto activeScene = EditorSceneManager::GetActiveScene();
        activeScene->OnSkyboxItemReceived(skybox);
    }
};