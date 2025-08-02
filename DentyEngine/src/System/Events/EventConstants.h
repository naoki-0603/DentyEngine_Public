#pragma once

namespace DentyEngine
{
	enum class EventType
	{
		LayerInitialized,

		// Script
		BeforeScriptAssemblyReload,
		ScriptAssemblyReloaded,

		// Application layer.
		OnApplicationStateChangedToEdit,
		OnApplicationStateChangedToPlay,
		OnApplicationStateChangedToPause,

		RuntimeSceneChange,
		RuntimeSceneChanged,

		// Window Events
		WindowClosed,
		WindowResized,

		// Mouse Events
		MouseButtonPressed,
		MouseButtonReleased,
		MouseButtonTriggered,
		MouseScrolled,
		MouseMoved,

		// Keyboard Events
		KeyPressed,
		KeyReleased,
		KeyTriggered,

		// GamePad Events
		GamePadButtonPressed,
		GamePadButtonReleased,
		GamePadButtonTriggered,

		// Rendering Events
		TellRenderBulletDebug,

		EditorBeginRendering,
		EditorEndRendering,

		GameBeginRendering,
		GameEndRendering,

		SceneRendering,

		BeginGUIRendering,
		EndGUIRendering,

		Present,

		RendererCreated,
		RendererDestroyed,
		LightCreated,
		LightDestroyed,

		OnMeshChanged,

		RequiredSceneViewColorData,
		RequiredGameViewColorData,
		RequiredEditorCameraData,

		SceneViewSizeChanged,
		GameViewSizeChanged,

		// Editor events
		SelectedGameObject,
		OnFocusKeyPressed,

		// Tool bar
		OnScenePlayButtonPressed,
		OnSceneResumeButtonPressed,
		OnScenePauseButtonPressed,
		OnSceneStepButtonPressed,

		// Menu bar
		OnSettingsWindowOpened,
		OnSettingsWindowClosed,

		OnSceneManagerWindowOpened,
		OnSceneManagerWindowClosed,

		OnAnimatorWindowOpened,
		OnAnimatorWindowClosed,

		OnCollisionMeshWindowOpened,
		OnCollisionMeshWindowClosed,

		OnGraphicsWindowOpened,
		OnGraphicsWindowClosed,

		OnPostEffectWindowOpened,
		OnPostEffectWindowClosed,

		OnProfilerWindowOpened,
		OnProfilerWindowClosed,

		ProjectSettingWindowOpen,
		ProjectSettingWindowClose,

		// Asset editor.
		OnMaterialEditorOpened,
		OnMaterialEditorClosed,

		OnMeshEditorOpened,
		OnMeshEditorClosed,

		OnShaderEditorOpened,
		OnShaderEditorClosed,

		OnTextureEditorOpened,
		OnTextureEditorClosed,

		OnPrefabEditorOpened,
		OnPrefabEditorClosed,

		// Each Asset window events.

		// Material editor.
		OnMaterialParameterChanged,

		// Assets updated event.
		OnMaterialAssetUpdated,
		OnPrefabAssetUpdated,
		OnSceneAssetUpdated,
		OnMeshAssetUpdated,

		OnShadingTypeChanged,

		PrefabApplyChangesButtonPressed,

		// Guizmos
		RenderManipulateGizmo,
		RenderIconGuizmo,
		RenderOutlineGuizmo,

		// Scene events
		SceneCreated,
		SceneSaved,
		SceneLoaded,

		RequestedSceneLoad,
		RequestedSceneSave,
		RequestedSceneSaveAs,

		SceneActivated,
		SceneDeActivated,

		CreatedGameObject,
		CreatedGameObjectInHierarchy,

		OnGameObjectDestroyed,
		OnGameObjectDeserialized,

		// GameObject events.

		OnComponentAdded,
		OnComponentRemoved,

		OnParentChildRelationCreated,
		OnParentChildRelationDestroyed,

		OnComponentDestroyed,

		OnComponentSerialized,
		OnComponentDeserialized,

		OnPrefabOverrideButtonClick,

		TagChanged,
		GameObjectEnabled,
		GameObjectDisabled,

		// Physics events.
		OnColliderCreated,
		OnRigidBodyCreated,
		OnCollisionShapeModified,

		OnColliderDestroyed,
		OnRigidBodyDestroyed,

		OnActionCreated,
		OnActionDestroyed,

		// Collision events.
		OnCollisionEnter,
		OnCollisionStay,
		OnCollisionExit,

		// Trigger events.
		OnTriggerEnter,
		OnTriggerStay,
		OnTriggerExit,
	};

	enum class EventInheritedType
	{
		Event,
		EventCollisionAndTrigger,

		Max
	};

	// Asset
	enum class EventAssetUpdatedOperationType
	{
		Rename,
		Deleted,
		ContentChanged,

		Max
	};
}