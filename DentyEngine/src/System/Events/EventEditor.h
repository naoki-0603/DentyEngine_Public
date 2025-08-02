#pragma once

#include "System/Core/DFilePath.h"
#include "Event.h"

#include "System/Core/ID/MeshID.h"
#include "System/Core/ID/SceneID.h"
#include "System/Core/ID/MaterialID.h"
#include "System/Core/ID/PrefabID.h"

namespace DentyEngine
{
	class AnimatorController;
	class Scene;
	class Texture;
	class Texture2D;
	class Shader;
	class Material;
	class GameObject;

	class Prefab;
	class Mesh;
	class CollisionMesh;

	// Hierarchy
	class EventSelectedGameObject final : public Event
	{
	public:
		EventSelectedGameObject(GameObject* gameObject) : Event(true), _selectedGameObject(gameObject) {}
		~EventSelectedGameObject() override = default;

		GameObject* GetGameObject() const { return _selectedGameObject; }

		EVENT_CLASS_TYPE(SelectedGameObject)
	private:
		GameObject* _selectedGameObject;
	};

	// F key is focus.
	class EventOnFocusKeyPressed final : public Event
	{
	public:
		EventOnFocusKeyPressed(const Vector3& focusPosition) : Event(false), _focusPosition(focusPosition) {}
		~EventOnFocusKeyPressed() override = default;

		const Vector3& GetFocusPosition() const { return _focusPosition; }

		EVENT_CLASS_TYPE(OnFocusKeyPressed)
	private:
		Vector3 _focusPosition;
	};

	// View
	class EventRenderManipulateGuizmo final : public Event
	{
	public:
		EventRenderManipulateGuizmo(const Vector2& viewportPosition, const Vector2& viewportSize) :
			Event(false), _viewportPosition(viewportPosition), _viewportSize(viewportSize) {}
		~EventRenderManipulateGuizmo() override = default;

		EVENT_CLASS_TYPE(RenderManipulateGizmo)

		[[nodiscard]]
		const Vector2& GetViewportPosition() const { return _viewportPosition; }

		[[nodiscard]]
		const Vector2& GetViewportSize() const { return _viewportSize; }
	private:
		Vector2 _viewportPosition;
		Vector2 _viewportSize;
	};

	class EventRenderIconGuizmo final : public Event
	{
	public:
		EventRenderIconGuizmo(Texture2D* texture) :
			Event(false), _texture(texture) {}
		~EventRenderIconGuizmo() override = default;

		Texture2D* GetTexture() const { return _texture; }

		EVENT_CLASS_TYPE(RenderIconGuizmo)
	private:
		Texture2D* _texture;
	};

	class EventRenderOutlineGuizmo final : public Event
	{
	public:
		EventRenderOutlineGuizmo() : Event(false) {}
		~EventRenderOutlineGuizmo() override = default;

		EVENT_CLASS_TYPE(RenderOutlineGuizmo)
	};

	// Tool bar
	class EventOnScenePlayButtonPressed final : public Event
	{
	public:
		EventOnScenePlayButtonPressed() : Event(true) {}
		~EventOnScenePlayButtonPressed() override = default;

		EVENT_CLASS_TYPE(OnScenePlayButtonPressed)
	};

	class EventOnSceneResumeButtonPressed final : public Event
	{
	public:
		EventOnSceneResumeButtonPressed() : Event(true) {}
		~EventOnSceneResumeButtonPressed() override = default;

		EVENT_CLASS_TYPE(OnSceneResumeButtonPressed)
	};

	class EventOnScenePauseButtonPressed final : public Event
	{
	public:
		EventOnScenePauseButtonPressed() : Event(true) {}
		~EventOnScenePauseButtonPressed() override = default;

		EVENT_CLASS_TYPE(OnScenePauseButtonPressed)
	};

	class EventOnSceneStepButtonPressed final : public Event
	{
	public:
		EventOnSceneStepButtonPressed() : Event(true) {}
		~EventOnSceneStepButtonPressed() override = default;

		EVENT_CLASS_TYPE(OnSceneStepButtonPressed)
	};

	// Content browser.

	// Menu bar.
	class EventOnSettingsWindowOpened final : public Event
	{
	public:
		EventOnSettingsWindowOpened() :
		    Event(false) {}
		~EventOnSettingsWindowOpened() override = default;

		EVENT_CLASS_TYPE(OnSettingsWindowOpened)
	};

	class EventOnSettingsWindowClosed final : public Event
	{
	public:
		EventOnSettingsWindowClosed() :
		    Event(false) {}
		~EventOnSettingsWindowClosed() override = default;

		EVENT_CLASS_TYPE(OnSettingsWindowClosed)
	};

	class EventOnSceneManagerWindowOpened final : public Event
	{
	public:
		EventOnSceneManagerWindowOpened() :
			Event(false) {}
		~EventOnSceneManagerWindowOpened() override = default;

		EVENT_CLASS_TYPE(OnSceneManagerWindowOpened)
	};

	class EventOnSceneManagerWindowClosed final : public Event
	{
	public:
		EventOnSceneManagerWindowClosed() : Event(false) {}
		~EventOnSceneManagerWindowClosed() override = default;

		EVENT_CLASS_TYPE(OnSceneManagerWindowClosed)
	};

	class EventOnAnimatorWindowOpened final : public Event
	{
	public:
		EventOnAnimatorWindowOpened(const Ref<AnimatorController>& controller)
			: Event(false), _controller(controller) {}
		~EventOnAnimatorWindowOpened() override = default;

		[[nodiscard]]
		Ref<AnimatorController> GetController() const { return _controller; }

		EVENT_CLASS_TYPE(OnAnimatorWindowOpened)
	private:
		Ref<AnimatorController> _controller;
	};

	class EventOnAnimatorWindowClosed final : public Event
	{
	public:
		EventOnAnimatorWindowClosed() : Event(true) {}
		~EventOnAnimatorWindowClosed() override = default;

		EVENT_CLASS_TYPE(OnAnimatorWindowClosed)
	};

	class EventOnCollisionMeshWindowOpened final : public Event
	{
	public:
		EventOnCollisionMeshWindowOpened(Ref<CollisionMesh> collisionMesh) :
			Event(false), _collisionMesh(collisionMesh) {}
		~EventOnCollisionMeshWindowOpened() override = default;

		[[nodiscard]]
		Ref<CollisionMesh> GetCollisionMesh() const { return _collisionMesh; }

		EVENT_CLASS_TYPE(OnCollisionMeshWindowOpened)
	private:
		Ref<CollisionMesh> _collisionMesh;
	};

	class EventOnCollisionMeshWindowClosed final : public Event
	{
	public:
		EventOnCollisionMeshWindowClosed(MeshID id) : Event(true), _id(id) {}
		~EventOnCollisionMeshWindowClosed() override = default;

		[[nodiscard]]
		MeshID GetMeshID() const { return _id; }

		EVENT_CLASS_TYPE(OnCollisionMeshWindowClosed)
	private:
		MeshID _id;
	};

	class EventOnGraphicsWindowOpened final : public Event
	{
	public:
		EventOnGraphicsWindowOpened()
			: Event(false) {}
		~EventOnGraphicsWindowOpened() override = default;

		EVENT_CLASS_TYPE(OnGraphicsWindowOpened)
	};

	class EventOnGraphicsWindowClosed final : public Event
	{
	public:
		EventOnGraphicsWindowClosed() : Event(true) {}
		~EventOnGraphicsWindowClosed() override = default;

		EVENT_CLASS_TYPE(OnGraphicsWindowClosed)
	};


	class EventOnPostEffectWindowOpened final : public Event
	{
	public:
		EventOnPostEffectWindowOpened() : Event(false) {}
		~EventOnPostEffectWindowOpened() override = default;

		EVENT_CLASS_TYPE(OnPostEffectWindowOpened)
	};

	class EventOnPostEffectWindowClosed final : public Event
	{
	public:
		EventOnPostEffectWindowClosed() : Event(true) {}
		~EventOnPostEffectWindowClosed() override = default;

		EVENT_CLASS_TYPE(OnPostEffectWindowClosed)
	};

	class EventOnProfilerWindowOpened final : public Event
	{
	public:
		EventOnProfilerWindowOpened() : Event(false) {}
		~EventOnProfilerWindowOpened() override = default;

		EVENT_CLASS_TYPE(OnProfilerWindowOpened)
	};

	class EventOnProfilerWindowClosed final : public Event
	{
	public:
		EventOnProfilerWindowClosed() : Event(true) {}
		~EventOnProfilerWindowClosed() override = default;

		EVENT_CLASS_TYPE(OnProfilerWindowClosed)
	};

	class EventProjectSettingWindowOpen final : public Event
	{
	public:
		EventProjectSettingWindowOpen() : Event(false) {}
		~EventProjectSettingWindowOpen() override = default;

		EVENT_CLASS_TYPE(ProjectSettingWindowOpen)
	};

	class EventProjectSettingWindowClose final : public Event
	{
	public:
		EventProjectSettingWindowClose() : Event(false) {}
		~EventProjectSettingWindowClose() override = default;

		EVENT_CLASS_TYPE(ProjectSettingWindowClose)
	};

	// Asset window.
	class EventOnMaterialEditorOpened final : public Event
	{
	public:
		EventOnMaterialEditorOpened(const Ref<Material>& material) :
			Event(false), _material(material) {}
		~EventOnMaterialEditorOpened() override = default;

		[[nodiscard]]
		Ref<Material> GetMaterial() const { return _material; }

		EVENT_CLASS_TYPE(OnMaterialEditorOpened)
	private:
		Ref<Material> _material;
	};

	class EventOnMaterialEditorClosed final : public Event
	{
	public:
		EventOnMaterialEditorClosed(const FilePath& filePath, bool isChanged) :
	        Event(true), _filePath(filePath), _isChanged(isChanged) {}
		~EventOnMaterialEditorClosed() override = default;

		[[nodiscard]]
		const FilePath& GetFilePath() const { return _filePath; }

		[[nodiscard]]
		bool GetIsChanged() const { return _isChanged; }

		EVENT_CLASS_TYPE(OnMaterialEditorClosed)
	private:
		FilePath _filePath;
		bool _isChanged;
	};

	class EventOnMeshEditorOpened final : public Event
	{
	public:
		EventOnMeshEditorOpened(const Ref<Mesh>& mesh) :
			Event(false), _mesh(mesh) {}
		~EventOnMeshEditorOpened() override = default;

		[[nodiscard]]
		Ref<Mesh> GetMesh() const { return _mesh; }

		EVENT_CLASS_TYPE(OnMeshEditorOpened)
	private:
		Ref<Mesh> _mesh;
	};

	class EventOnMeshEditorClosed final : public Event
	{
	public:
		EventOnMeshEditorClosed(MeshID id) : Event(true), _id(id) {}
		~EventOnMeshEditorClosed() override = default;

		[[nodiscard]]
		MeshID GetMeshID() const { return _id; }

		EVENT_CLASS_TYPE(OnMeshEditorClosed)
	private:
		MeshID _id;
	};

	class EventOnShaderEditorOpened final : public Event
	{
	public:
		EventOnShaderEditorOpened(const Ref<Shader>& shader) :
			Event(false), _shader(shader) {}
		~EventOnShaderEditorOpened() override = default;

		[[nodiscard]]
	    Ref<Shader> GetShader() const { return _shader; }

		EVENT_CLASS_TYPE(OnShaderEditorOpened)
	private:
		Ref<Shader> _shader;
	};

	class EventOnShaderEditorClosed final : public Event
	{
	public:
		EventOnShaderEditorClosed() : Event(false) {}
		~EventOnShaderEditorClosed() override = default;

		EVENT_CLASS_TYPE(OnShaderEditorClosed)
	};

	class EventOnTextureEditorOpened final : public Event
	{
	public:
		EventOnTextureEditorOpened(const Ref<Texture>& texture) : Event(false), _texture(texture) {}
		~EventOnTextureEditorOpened() override = default;

		[[nodiscard]]
		const Ref<Texture>& GetTexture() const { return _texture; }

		EVENT_CLASS_TYPE(OnTextureEditorOpened)
	private:
		Ref<Texture> _texture;
	};

	class EventOnTextureEditorClosed final : public Event
	{
	public:
		EventOnTextureEditorClosed() : Event(false) {}
		~EventOnTextureEditorClosed() override = default;

		EVENT_CLASS_TYPE(OnTextureEditorClosed)
	};

	class EventOnPrefabEditorOpened final : public Event
	{
	public:
		EventOnPrefabEditorOpened(const Ref<Prefab>& prefab) :
			Event(false), _prefab(prefab) {}
		~EventOnPrefabEditorOpened() override = default;

		[[nodiscard]]
		Ref<Prefab> GetPrefab() const { return _prefab; }

		EVENT_CLASS_TYPE(OnPrefabEditorOpened)
	private:
		Ref<Prefab> _prefab;
	};

	class EventOnPrefabEditorClosed final : public Event
	{
	public:
		EventOnPrefabEditorClosed(const FilePath& filePath) : Event(true), filePath( filePath ) {}
		~EventOnPrefabEditorClosed() override = default;

		EVENT_CLASS_TYPE(OnPrefabEditorClosed)
	public:
		FilePath filePath;
	};


	// Each Asset window events.

	// Material editor.
	class EventOnMaterialParameterChanged final : public Event
	{
	public:
		EventOnMaterialParameterChanged(MaterialID id) : Event(true), _id(id) {}
		~EventOnMaterialParameterChanged() override = default;

		[[nodiscard]]
		MaterialID GetMaterialID() const { return _id; }

		EVENT_CLASS_TYPE(OnMaterialParameterChanged)
	private:
		MaterialID _id;
	};

	// Updated asset events.

	// Base class for updated asset event.
	class EventAssetUpdated : public Event
	{
	public:
		EventAssetUpdated(std::string_view newFilePath, EventAssetUpdatedOperationType type) : Event(true),
			_newFilePath(newFilePath), _type(type) {}
		virtual ~EventAssetUpdated() override = default;

		[[nodiscard]]
		const std::string& GetNewFilePath() const { return _newFilePath; }

		[[nodiscard]]
		EventAssetUpdatedOperationType GetOperationType() const { return _type; }
	protected:
		std::string _newFilePath;

		EventAssetUpdatedOperationType _type;
	};

	class EventOnMaterialAssetUpdated final : public EventAssetUpdated
	{
	public:
		EventOnMaterialAssetUpdated(std::string_view newFilePath, MaterialID id, EventAssetUpdatedOperationType type) : EventAssetUpdated(newFilePath, type), _id(id) {}
		~EventOnMaterialAssetUpdated() override = default;

		[[nodiscard]]
		MaterialID GetMaterialID() const { return _id; }

		EVENT_CLASS_TYPE(OnMaterialAssetUpdated)
	private:
		MaterialID _id;
	};

	class EventOnPrefabAssetUpdated final : public EventAssetUpdated
	{
	public:
		EventOnPrefabAssetUpdated(std::string_view newFilePath, PrefabID id, EventAssetUpdatedOperationType type) : EventAssetUpdated(newFilePath, type), _id(id) {}
		~EventOnPrefabAssetUpdated() override = default;

		[[nodiscard]]
		PrefabID GetPrefabID() const { return _id; }

		EVENT_CLASS_TYPE(OnPrefabAssetUpdated)
	private:
		PrefabID _id;
	};

	class EventOnSceneAssetUpdated final : public EventAssetUpdated
	{
	public:
		EventOnSceneAssetUpdated(std::string_view newFilePath, SceneID id, EventAssetUpdatedOperationType type) : EventAssetUpdated(newFilePath, type), _id(id) {}
		~EventOnSceneAssetUpdated() override = default;

		[[nodiscard]]
		SceneID GetSceneID() const { return _id; }

		EVENT_CLASS_TYPE(OnSceneAssetUpdated)
	private:
		SceneID _id;
	};

	class EventOnMeshAssetUpdated final : public EventAssetUpdated
	{
	public:
		EventOnMeshAssetUpdated(std::string_view newFilePath, MeshID id, EventAssetUpdatedOperationType type) : EventAssetUpdated(newFilePath, type), _id(id) {}
		~EventOnMeshAssetUpdated() override = default;

		[[nodiscard]]
		MeshID GetMeshID() const { return _id; }

		EVENT_CLASS_TYPE(OnMeshAssetUpdated)
	private:
		MeshID _id;
	};

	//
	// Asset deleted event.
	//


	class EventOnShadingTypeChanged final : public Event
	{
	public:
		EventOnShadingTypeChanged(int shadingType) : Event(true), _shadingType(shadingType) {}
		~EventOnShadingTypeChanged() override = default;

		EVENT_CLASS_TYPE(OnShadingTypeChanged)
	private:
		int _shadingType;
	};

	class EventPrefabApplyChangesButtonPressed final : public Event
	{
	public:
		enum class ChangeType : int32_t
		{
			None = -1,
			Tag = 0,
		};
	public:
		EventPrefabApplyChangesButtonPressed(const FilePath& prefabFilePathToSerialized, ChangeType changeType)
			: Event(false), _filePath(prefabFilePathToSerialized), _changeType(changeType) {}
		~EventPrefabApplyChangesButtonPressed() override = default;

		[[nodiscard]]
		const FilePath& GetFilePath() const { return _filePath; }

		[[nodiscard]]
		ChangeType GetChangeType() const { return _changeType; }

		EVENT_CLASS_TYPE(PrefabApplyChangesButtonPressed)
	private:
		FilePath _filePath;

		ChangeType _changeType;
	};

}
