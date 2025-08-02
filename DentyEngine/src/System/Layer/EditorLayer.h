#pragma once

#include "../Layer/Layer.h"

#include "Editor/Parts/EditorContentBrowser.h"
#include "Editor/Parts/EditorGameView.h"
#include "Editor/Parts/EditorHierarchy.h"
#include "Editor/Parts/EditorInspector.h"
#include "Editor/Parts/EditorMenuBar.h"
#include "Editor/Parts/EditorSceneSettings.h"
#include "Editor/Parts/EditorSceneView.h"
#include "Editor/Parts/EditorToolBar.h"

#include "Editor/Parts/Windows/EditorSettingsWindow.h"
#include "Editor/Parts/Windows/EditorAnimatorWindow.h"
#include "Editor/Parts/Windows/EditorCollisionMeshWindow.h"
#include "Editor/Parts/Windows/EditorSceneManagerWindow.h"
#include "Editor/Parts/Windows/EditorPostEffectWindow.h"
#include "Editor/Parts/Windows/EditorProfilerWindow.h"
#include "Editor/Parts/Windows/EditorProjectSettingWindow.h"

#include "Editor/Parts/Assets/MaterialEditor.h"
#include "Editor/Parts/Assets/ShaderEditor.h"
#include "Editor/Parts/Assets/PrefabEditor.h"
#include "Editor/Parts/Assets/MeshEditor.h"
#include "Editor/Parts/Assets/TextureEditor.h"

#include "Editor/Parts/Windows/EditorGraphicsWindow.h"

namespace DentyEngine
{
	class EventOnSceneActivated;
	class EventOnSceneDeActivated;

	class EventOnScenePlayButtonPressed;

	class EditorLayer final : public Layer
	{
	public:
		EditorLayer(const bool enabled);
		~EditorLayer() override = default;

		void OnAttach() override;
		void OnUpdate(const float deltaTime) override;
		void OnLateUpdate(float deltaTime) override;
		void OnFixedUpdate() override;
		void OnDetach() override;
		void OnGui() override;
		void OnEvent(Event* e) override;

		//
		// Events
		//
		void OnSceneActivated(EventOnSceneActivated* e);
		void OnSceneDeActivated(EventOnSceneDeActivated* e);

		void OnScenePlayButtonPressed(EventOnScenePlayButtonPressed* e);

		static void Begin();
		static void End();
	private:
		static void SetDockspace();

		void Settings();
		void SetStyle();
	private:
		//
		// Main Editor windows
		//
		Scope<EditorMenuBar> _menuBar;
		Scope<EditorToolBar> _toolBar;
		Scope<EditorHierarchy> _hierarchy;
		Scope<EditorInspector> _inspector;
		Scope<EditorGameView> _gameView;
		Scope<EditorSceneView> _sceneView;
		Scope<EditorContentBrowser> _contentBrowser;
		Scope<EditorSceneSettings> _sceneSettings;

		//
		// Sub editor windows.
		//
		Scope<EditorSettingsWindow> _settingsWindow;
		Scope<EditorAnimatorWindow> _animatorWindow;
		Scope<EditorCollisionMeshWindow> _collisionMeshWindow;
		Scope<EditorGraphicsWindow> _graphicsWindow;
		Scope<EditorSceneManagerWindow> _sceneManagerWindow;
		//Scope<EditorPostEffectWindow> _postEffectWindow;
		Scope<EditorProfilerWindow> _profilerWindow;
		Scope<EditorProjectSettingWindow> _projectSettingWindow;

		//
		// Asset editor windows.
		//
		Scope<MaterialEditor> _materialEditor;
		Scope<ShaderEditor> _shaderEditor;
		Scope<PrefabEditor> _prefabEditor;
		Scope<MeshEditor> _meshEditor;
		Scope<TextureEditor> _textureEditor;

		Ref<Scene> _activeScene;

		bool _sceneOpened;
	};
};