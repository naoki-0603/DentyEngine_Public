#include "stdafx.h"

#include "EditorLayer.h"

#include "System/Core/Window.h"

#include "../Graphics/GraphicsManager.h"

#include "System/Core/ImGuiConsole.h"

#include "System/Events/EventScene.h"
#include "System/Events/EventEditor.h"

#include "ApplicationLayer.h"

// Components
#include "Component/Graphics/Renderer/DirectionalLight.h"
#include "Component/Graphics/Renderer/PointLight.h"
#include "Component/Graphics/Renderer/SpotLight.h"
#include "Component/Graphics/Renderer/HemisphereLight.h"

#include "Component/Graphics/Renderer/GameCamera.h"

#include "Component/Graphics/Renderer/MeshRenderer.h"
#include "Component/Graphics/Renderer/SkinnedMeshRenderer.h"

#include "Component/Graphics/Renderer/EffectComponent.h"

#include "Component/Physics/Colliders/BoxCollider.h"
#include "Component/Physics/Colliders/SphereCollider.h"
#include "Component/Physics/Colliders/CapsuleCollider.h"
#include "Component/Physics/Colliders/MeshCollider.h"

#include "Component/Physics/RigidBody.h"

#include "Component/Animations/Animator.h"

#include "Component/Controller/DefaultCameraController.h"

#include "Component/Physics/DynamicCharacterController.h"
#include "Component/Physics/KinematicCharacterController.h"
#include "Component/Physics/Colliders/ApproximationMeshCollider.h"
#include "Component/Physics/Colliders/ConvexMeshCollider.h"
#include "Component/Physics/Vehicle/VehicleComponent.h"

#include "Component/Scripts/ScriptComponent.h"

#include "Component/Game/Race/CheckPoint.h"

#include "Graphics/Skybox.h"

#include "Graphics/Mesh/SceneTree.h"
#include "Graphics/Mesh/Skeleton.h"

#include "Graphics/Buffer/SceneBuffer.h"
#include "Graphics/Renderer/CubeMaterial.h"
#include "Graphics/Sprite/Sprite3D.h"
#include "Physics/ApproximationCollisionMesh.h"

#include "Physics/CollisionMesh.h"

#include "System/Graphics/PostEffect/PostEffectGenerator.h"
#include "System/Resource/Prefab.h"
#include "System/Resource/Script.h"
#include "System/Scene/Scene.h"

#include "Editor/Graphics/Rendering/EditorCamera.h"
#include "System/Editor/EditorSceneManager.h"

namespace DentyEngine
{
	EditorLayer::EditorLayer(const bool enabled) : Layer("EditorLayer", enabled),
		_menuBar(), _toolBar(), _hierarchy(), _inspector(), _gameView(),
		_sceneView(), _contentBrowser(), _sceneSettings(), _settingsWindow(), _animatorWindow(), _collisionMeshWindow(),
		_graphicsWindow(), _sceneManagerWindow(), _profilerWindow(), _projectSettingWindow(),
		_materialEditor(), _shaderEditor(), _prefabEditor(), _meshEditor(), _textureEditor(), _activeScene(),
		_sceneOpened()
	{
	}

    void EditorLayer::OnAttach()
    {
        ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

        // Create editor parts.
        {
            _menuBar = std::make_unique<EditorMenuBar>();
            _toolBar = std::make_unique<EditorToolBar>(device);
            _hierarchy = std::make_unique<EditorHierarchy>();
            _inspector = std::make_unique<EditorInspector>();
            _gameView = std::make_unique<EditorGameView>();
            _sceneView = std::make_unique<EditorSceneView>(device);
            _contentBrowser = std::make_unique<EditorContentBrowser>(device);
            _sceneSettings = std::make_unique<EditorSceneSettings>();
        }

        // Create sub editor windows.
        {
            _settingsWindow = std::make_unique<EditorSettingsWindow>();
            _animatorWindow = std::make_unique<EditorAnimatorWindow>();
            _collisionMeshWindow = std::make_unique<EditorCollisionMeshWindow>();
        	_graphicsWindow = std::make_unique<EditorGraphicsWindow>();
            _sceneManagerWindow = std::make_unique<EditorSceneManagerWindow>();
            //_postEffectWindow = std::make_unique<EditorPostEffectWindow>();
            _profilerWindow = std::make_unique<EditorProfilerWindow>();
            _projectSettingWindow = std::make_unique<EditorProjectSettingWindow>();
        }

        // Create assets editor windows.
        {
            _materialEditor = std::make_unique<MaterialEditor>();
            _shaderEditor = std::make_unique<ShaderEditor>();
            _prefabEditor = std::make_unique<PrefabEditor>();
            _meshEditor = std::make_unique<MeshEditor>();
            _textureEditor = std::make_unique<TextureEditor>();
        }

        // Create add component list.
        {
            // Graphics
            {
                DirectionalLight::RegisterBuildInComponent();
                PointLight::RegisterBuildInComponent();
                SpotLight::RegisterBuildInComponent();
                HemisphereLight::RegisterBuildInComponent();

                GameCamera::RegisterBuildInComponent();

                MeshRenderer::RegisterBuildInComponent();
                SkinnedMeshRenderer::RegisterBuildInComponent();

                EffectComponent::RegisterBuildInComponent();
            }

            // Physics
            {
                BoxCollider::RegisterBuildInComponent();
                SphereCollider::RegisterBuildInComponent();
                CapsuleCollider::RegisterBuildInComponent();
                MeshCollider::RegisterBuildInComponent();
                ConvexMeshCollider::RegisterBuildInComponent();
                ApproximationMeshCollider::RegisterBuildInComponent();

                RigidBody::RegisterBuildInComponent();
                KinematicCharacterController::RegisterBuildInComponent();
                DynamicCharacterController::RegisterBuildInComponent();

                VehicleComponent::RegisterBuildInComponent();
            }

            // Controller
            {
                DefaultCameraController::RegisterBuildInComponent();
            }

            // Animations
            {
                Animator::RegisterBuildInComponent();
            }

            // Script
            {
                ScriptComponent::RegisterBuildInComponent();
            }

            // Game
            {
	            // Race
                {
                    CheckPoint::RegisterBuildInComponent();
                }
            }
        }

        // Load or setup asset.
        {
            Material::Setup();
            CubeMaterial::Setup();

            Sprite3D::Setup();

            Scene::LoadAssetTexture();
            Prefab::LoadAssetTexture();
            Shader::LoadAssetTexture();
            Mesh::LoadAssetTexture();
            CollisionMesh::LoadAssetTexture();
            ApproximationCollisionMesh::LoadAssetTexture();
            MeshResource::SceneTree::LoadAssetTexture();
            MeshResource::Skeleton::LoadAssetTexture();
            MeshResource::AnimationClip::LoadAssetTexture();
            AnimatorController::LoadAssetTexture();
            Skybox::LoadAssetTexture();
            Script::LoadAssetTexture();
        }

        // Post effect generator.
        {
            PostEffectGenerator::Initialize();
        }

        // Settings
        {
            Settings();
            SetStyle();
        }

        // Set title scene.
        EditorSceneManager::SetReservedSceneFilePath(
            "Assets\\Scenes\\Crystal_Game.DentyScene",
            false,
            false,
            false
        );

        _initialized = true;
    }

    void EditorLayer::OnUpdate(const float deltaTime)
    {
        _sceneView->OnUpdate(deltaTime);
        _sceneSettings->OnUpdate();
    }

    void EditorLayer::OnLateUpdate([[maybe_unused]] float deltaTime)
    {
        // Nothing to do.
    }

    void EditorLayer::OnFixedUpdate()
    {
    }

    void EditorLayer::OnDetach()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void EditorLayer::OnGui()
    {
        _menuBar->OnGui(_activeScene);

        // Scene is open and active scene exists.
        if (_sceneOpened && _activeScene)
        {
            _toolBar->OnGui(_activeScene);
            _hierarchy->OnGui(_activeScene);
            _inspector->OnGui(_activeScene);

            _gameView->OnGui(_activeScene);

            _sceneView->OnGui(_activeScene);
            _contentBrowser->OnGui(_activeScene);
            _sceneSettings->OnGui(_activeScene);

            ImGuiConsole::OnGui();
        }

        // EditorWindows
        {
            // SettingsWindow.
            _settingsWindow->OnGui(_activeScene);
            _animatorWindow->OnGui(_activeScene);
            _collisionMeshWindow->OnGui(_activeScene);
            _graphicsWindow->OnGui(_activeScene);
            _sceneManagerWindow->OnGui(_activeScene);
            _profilerWindow->OnGui(_activeScene);
            _projectSettingWindow->OnGui(_activeScene);
        }

        // AssetEditorWindows
        {
	        if (_materialEditor->AssetExists())
	        {
                _materialEditor->OnGui(_activeScene);
	        }

            if (_shaderEditor->AssetExists())
            {
                _shaderEditor->OnGui(_activeScene);
            }

            if (_prefabEditor->AssetExists())
            {
                _prefabEditor->OnGui(_activeScene);
            }

            if (_meshEditor->AssetExists())
            {
                _meshEditor->OnGui(_activeScene);
            }

            if (_textureEditor->AssetExists())
            {
                _textureEditor->OnGui(_activeScene);
            }
        }
    }

    void EditorLayer::OnEvent(Event* e)
    {
        Layer::OnEvent(e);

        // If not initialized, return.
        if (not _initialized) return;

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<EventOnSceneActivated>(BIND_EVENT_FUNCTION(OnSceneActivated));
        dispatcher.Dispatch<EventOnSceneDeActivated>(BIND_EVENT_FUNCTION(OnSceneDeActivated));

    	dispatcher.Dispatch<EventOnScenePlayButtonPressed>(BIND_EVENT_FUNCTION(OnScenePlayButtonPressed));

        // Main Editor windows.
        {
            _menuBar->OnEvent(e);
            _toolBar->OnEvent(e);
            _hierarchy->OnEvent(e);
            _inspector->OnEvent(e);
            _gameView->OnEvent(e);

            // Also called editor camera on event.
            _sceneView->OnEvent(e);
            _contentBrowser->OnEvent(e);
            _sceneSettings->OnEvent(e);
        }

        // Sub Editor Windows.
        {
            _settingsWindow->OnEvent(e);
            _animatorWindow->OnEvent(e);
            _collisionMeshWindow->OnEvent(e);
            _graphicsWindow->OnEvent(e);
            _sceneManagerWindow->OnEvent(e);
            _profilerWindow->OnEvent(e);
            _projectSettingWindow->OnEvent(e);
        }

        // Asset editor windows.
        {
            _materialEditor->OnEvent(e);
            _shaderEditor->OnEvent(e);
            _prefabEditor->OnEvent(e);
            _meshEditor->OnEvent(e);
            _textureEditor->OnEvent(e);
        }
    }

    void EditorLayer::OnSceneActivated(EventOnSceneActivated* e)
    {
        e->SetScene(_activeScene);
        _sceneOpened = true;
    }

    void EditorLayer::OnSceneDeActivated([[maybe_unused]] EventOnSceneDeActivated* e)
    {
        _sceneOpened = false;
        _activeScene = nullptr;
    }

    void EditorLayer::OnScenePlayButtonPressed([[maybe_unused]] EventOnScenePlayButtonPressed* e)
    {
        ImGuiConsole::Clear();
    }

    void EditorLayer::Begin()
    {
    	ImGui_ImplDX11_NewFrame();
    	ImGui_ImplWin32_NewFrame();
    	ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        SetDockspace();

        ImGui::ShowDemoWindow();
    }

    void EditorLayer::End()
    {
        ImGui::End();

        ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void EditorLayer::SetDockspace()
    {
        static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

        // Copy from imgui_demo.cpp
        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        {
            windowFlags |= ImGuiWindowFlags_NoBackground;
        }

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        bool open = true;
        ImGui::Begin("Dockspace", &open, windowFlags);

        ImGui::PopStyleVar(2);

        // Submit the DockSpace
        const ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
        }
    }

    void EditorLayer::Settings()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();

        // Disable imgui.ini
        //-----------------------------
        // io.IniFilename = nullptr;
        // io.WantSaveIniSettings = false;
        //-----------------------------

        // Enable docking.
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui_ImplWin32_Init(Window::GetInfo().GetHwnd());
        ImGui_ImplDX11_Init(GraphicsManager::GetInstance().GetDevice(), GraphicsManager::GetInstance().GetContext());
    }

    void EditorLayer::SetStyle()
    {
        const auto& io = ImGui::GetIO();

        io.Fonts->Clear();
        io.Fonts->AddFontFromFileTTF("./Assets/Font/GenShinGothic-Medium.ttf", 16.0f, nullptr, glyphRangesJapanese);

        {
            constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b)
            {
                return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
            };

            auto& style = ImGui::GetStyle();
            ImVec4* colors = style.Colors;

            constexpr ImVec4 bgColor = ColorFromBytes(37, 37, 38);
            constexpr ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
            constexpr ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

            constexpr ImVec4 panelColor = ColorFromBytes(51, 51, 55);
            constexpr ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
            constexpr ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

            constexpr ImVec4 textColor = ColorFromBytes(255, 255, 255);
            constexpr ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
            constexpr ImVec4 borderColor = ColorFromBytes(78, 78, 78);

            colors[ImGuiCol_Text] = textColor;
            colors[ImGuiCol_TextDisabled] = textDisabledColor;
            colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
            colors[ImGuiCol_WindowBg] = bgColor;
            colors[ImGuiCol_ChildBg] = bgColor;
            colors[ImGuiCol_PopupBg] = bgColor;
            colors[ImGuiCol_Border] = borderColor;
            colors[ImGuiCol_BorderShadow] = borderColor;
            colors[ImGuiCol_FrameBg] = panelColor;
            colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
            colors[ImGuiCol_FrameBgActive] = panelActiveColor;
            colors[ImGuiCol_TitleBg] = bgColor;
            colors[ImGuiCol_TitleBgActive] = bgColor;
            colors[ImGuiCol_TitleBgCollapsed] = bgColor;
            colors[ImGuiCol_MenuBarBg] = panelColor;
            colors[ImGuiCol_ScrollbarBg] = panelColor;
            colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
            colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
            colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
            colors[ImGuiCol_CheckMark] = panelActiveColor;
            colors[ImGuiCol_SliderGrab] = panelHoverColor;
            colors[ImGuiCol_SliderGrabActive] = panelActiveColor;
            colors[ImGuiCol_Button] = panelColor;
            colors[ImGuiCol_ButtonHovered] = panelHoverColor;
            colors[ImGuiCol_ButtonActive] = panelHoverColor;
            colors[ImGuiCol_Header] = panelColor;
            colors[ImGuiCol_HeaderHovered] = panelHoverColor;
            colors[ImGuiCol_HeaderActive] = panelActiveColor;
            colors[ImGuiCol_Separator] = borderColor;
            colors[ImGuiCol_SeparatorHovered] = borderColor;
            colors[ImGuiCol_SeparatorActive] = borderColor;
            colors[ImGuiCol_ResizeGrip] = bgColor;
            colors[ImGuiCol_ResizeGripHovered] = panelColor;
            colors[ImGuiCol_ResizeGripActive] = lightBgColor;
            colors[ImGuiCol_PlotLines] = panelActiveColor;
            colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
            colors[ImGuiCol_PlotHistogram] = panelActiveColor;
            colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
            colors[ImGuiCol_ModalWindowDimBg] = bgColor;
            colors[ImGuiCol_DragDropTarget] = bgColor;
            colors[ImGuiCol_NavHighlight] = bgColor;
            colors[ImGuiCol_DockingPreview] = panelActiveColor;
            colors[ImGuiCol_Tab] = bgColor;
            colors[ImGuiCol_TabActive] = panelActiveColor;
            colors[ImGuiCol_TabUnfocused] = bgColor;
            colors[ImGuiCol_TabUnfocusedActive] = panelActiveColor;
            colors[ImGuiCol_TabHovered] = panelHoverColor;

            style.WindowRounding = 0.0f;
            style.ChildRounding = 0.0f;
            style.FrameRounding = 0.0f;
            style.GrabRounding = 0.0f;
            style.PopupRounding = 0.0f;
            style.ScrollbarRounding = 0.0f;
            style.TabRounding = 0.0f;
        }
    }
};