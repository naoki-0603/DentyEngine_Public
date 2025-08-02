#pragma once

#include "Event.h"

#include "System/Core/ID/SceneID.h"
#include "System/Core/ID/EntityID.h"
#include "System/Core/ID/PrefabID.h"

namespace DentyEngine
{
	class Scene;
	class GameObject;

	class Component;
	class MonoBehaviour;

	class EventOnSceneCreated final : public Event
	{
	public:
		EventOnSceneCreated(const Ref<Scene>& newScene) : Event(false), _newScene(newScene) {}
		~EventOnSceneCreated() override = default;

		EVENT_CLASS_TYPE(SceneCreated)
	private:
		Ref<Scene> _newScene;

		friend class GameSceneManager;
		friend class EditorSceneManager;
	};

	class EventOnSceneSaved final : public Event
	{
	public:
		EventOnSceneSaved() : Event(true) { }
		~EventOnSceneSaved() override = default;

		EVENT_CLASS_TYPE(SceneSaved)
	};

	class EventOnSceneLoaded final : public Event
	{
	public:
		 EventOnSceneLoaded() : Event(true) {}
		 ~EventOnSceneLoaded() override = default;

		 EVENT_CLASS_TYPE(SceneLoaded)
	};

	class EventRequestedSceneLoad final : public Event
	{
	public:
		EventRequestedSceneLoad(const std::filesystem::path& filePath, [[maybe_unused]] bool removeCurrentSceneFromCaches = false) :
			Event(false), _filePath(filePath), _removeCurrentSceneFromCaches(removeCurrentSceneFromCaches) {}
		~EventRequestedSceneLoad() override = default;

		[[nodiscard]]
		const std::filesystem::path& GetFilePath() const { return _filePath; }

		[[nodiscard]]
		bool GetRemoveCurrentSceneFromCaches() const { return _removeCurrentSceneFromCaches; }

		EVENT_CLASS_TYPE(RequestedSceneLoad)
	private:
		std::filesystem::path _filePath;

		bool _removeCurrentSceneFromCaches;
	};

	class EventRequestedSceneSave final : public Event
	{
	public:
		EventRequestedSceneSave(SceneID sceneID, std::string_view oldName, std::string_view sceneName) :
			Event(false), _sceneID(sceneID), _oldName(oldName), _sceneName(sceneName) {}
		~EventRequestedSceneSave() override = default;

		SceneID GetSceneID() const { return _sceneID; }

		const std::string& GetOldSceneName() const { return _oldName; }
		const std::string& GetSceneName() const { return _sceneName; }

		EVENT_CLASS_TYPE(RequestedSceneSave)
	private:
		SceneID _sceneID;

		std::string _oldName;
		std::string _sceneName;
	};

	class EventRequestedSceneSaveAs final : public Event
	{
	public:
		EventRequestedSceneSaveAs(SceneID id, std::string_view newFileName) :
			Event(false), _id(id), _newFileName(newFileName) {}
		~EventRequestedSceneSaveAs() override = default;

		[[nodiscard]]
		SceneID GetSceneID() const { return _id; }

		[[nodiscard]]
		const std::string& GetNewFileName() const { return _newFileName; }
 
		EVENT_CLASS_TYPE(RequestedSceneSaveAs)
	private:
		SceneID _id;

		std::string _newFileName;
	};

	class EventOnSceneActivated final : public Event
	{
	public:
		EventOnSceneActivated(const Ref<Scene>& scene) : Event(true), _activeScene(scene) {}
		~EventOnSceneActivated() override = default;

		void SetScene(Ref<Scene>& scene) const { scene = _activeScene; }

		EVENT_CLASS_TYPE(SceneActivated)
	private:
		Ref<Scene> _activeScene;
	};

	class EventOnSceneDeActivated final : public Event
	{
	public:
		EventOnSceneDeActivated() : Event(true) {}
		~EventOnSceneDeActivated() override = default;

		EVENT_CLASS_TYPE(SceneDeActivated)
	};

	class EventOnCreatedGameObject final : public Event
	{
	public:
		EventOnCreatedGameObject(const Ref<GameObject>& gameObject)
			: Event(false), _gameObject(gameObject) {}
		~EventOnCreatedGameObject() override = default;

		[[nodiscard]]
		Ref<GameObject> GetGameObject() const { return _gameObject; }

		EVENT_CLASS_TYPE(CreatedGameObject)
	private:
		Ref<GameObject> _gameObject;
	};

	class EventOnCreatedGameObjectInHierarchy final : public Event
	{
	public:
		EventOnCreatedGameObjectInHierarchy(const Ref<GameObject>& gameObject) :
			Event(false), _gameObject(gameObject) {}
		~EventOnCreatedGameObjectInHierarchy() override = default;

		Ref<GameObject> GetGameObject() const { return _gameObject; }

		EVENT_CLASS_TYPE(CreatedGameObjectInHierarchy)
	private:
		Ref<GameObject> _gameObject;
	};

	class EventOnGameObjectDestroyed final : public Event
	{
	public:
		EventOnGameObjectDestroyed(EntityID id) : Event(true), _id(id) { }
		~EventOnGameObjectDestroyed() override = default;

		[[nodiscard]]
		EntityID GetID() const { return _id; }

		EVENT_CLASS_TYPE(OnGameObjectDestroyed)
	private:
		EntityID _id;
	};

	class EventOnGameObjectDeserialized final : public Event
	{
	public:
		EventOnGameObjectDeserialized(const Ref<GameObject>& gameObject) :
		    Event(true), _gameObject(gameObject) {}
		~EventOnGameObjectDeserialized() override = default;

		[[nodiscard]]
		Ref<GameObject> GetGameObject() const { return _gameObject; }

		EVENT_CLASS_TYPE(OnGameObjectDeserialized)
	private:
		Ref<GameObject> _gameObject;
	};

	class EventOnComponentAdded final : public Event
	{
	public:
		EventOnComponentAdded(const Ref<Component>& component) :
			Event(true), _component(component) { }
		~EventOnComponentAdded() override = default;

		Ref<Component> GetComponent() const { return _component; }

		EVENT_CLASS_TYPE(OnComponentAdded)
	private:
		Ref<Component> _component;
	};

	class EventOnComponentRemoved final : public Event
	{
	public:
		EventOnComponentRemoved(const Ref<Component>& component) :
			Event(false), _component(component) {}
		~EventOnComponentRemoved() override = default;

		[[nodiscard]] Ref<Component> GetComponent() const { return _component; }

		EVENT_CLASS_TYPE(OnComponentRemoved)
	private:
		Ref<Component> _component;
	};

	class EventOnParentChildRelationCreated final : public Event
	{
	public:
		EventOnParentChildRelationCreated(const EntityID entityID) :
			Event(true), _entityID(entityID) {}
		~EventOnParentChildRelationCreated() override = default;

		EntityID GetEntityID() const { return _entityID; }

		EVENT_CLASS_TYPE(OnParentChildRelationCreated)
	private:
		EntityID _entityID;
	};

	class EventOnParentChildRelationDestroyed final : public Event
	{
	public:
		EventOnParentChildRelationDestroyed(const EntityID entityID) :
			Event(true), _entityID(entityID) {}
		~EventOnParentChildRelationDestroyed() override = default;

		EntityID GetEntityID() const { return _entityID; }

		EVENT_CLASS_TYPE(OnParentChildRelationDestroyed)
	private:
		EntityID _entityID;
	};

	class EventOnComponentDestroyed final : public Event
	{
	public:
		EventOnComponentDestroyed(const Ref<Component>& component) :
			Event(false), _component(component) {}
		~EventOnComponentDestroyed() override = default;

		[[nodiscard]]
		Ref<Component> GetComponent() const { return _component; }

		EVENT_CLASS_TYPE(OnComponentDestroyed)
	private:
		Ref<Component> _component;
	};

	class EventOnComponentDeserialized final : public Event
	{
	public:
		EventOnComponentDeserialized(const Ref<Component>& component) :
		    Event(false), _component(component) {}
		~EventOnComponentDeserialized() override = default;

		[[nodiscard]]
		Ref<Component> GetComponent() const { return _component; }

		EVENT_CLASS_TYPE(OnComponentDeserialized)
	private:
		Ref<Component> _component;
	};

	class EventOnPrefabOverrideButtonClick final : public Event
	{
	public:
		EventOnPrefabOverrideButtonClick(PrefabID id, const Ref<GameObject>& sourceObject) : Event(true), _id(id), _sourceObject(sourceObject) {}
		~EventOnPrefabOverrideButtonClick() override = default;

		[[nodiscard]]
		PrefabID GetPrefabID() const { return _id; }

		[[nodiscard]]
		Ref<GameObject> GetSourceObject() const { return _sourceObject; }

		EVENT_CLASS_TYPE(OnPrefabOverrideButtonClick)
	private:
		PrefabID _id;

		Ref<GameObject> _sourceObject;
	};

	class EventGameObjectTagChanged final : public Event
	{
	public:
		EventGameObjectTagChanged(std::string_view tag) : Event(false), _tag(tag) {}
		~EventGameObjectTagChanged() override = default;

		[[nodiscard]]
		const std::string& GetTag() const { return _tag; }

		EVENT_CLASS_TYPE(TagChanged)
	private:
		std::string _tag;
	};

	class EventGameObjectEnabled final : public Event
	{
	public:
		EventGameObjectEnabled(Ref<GameObject> gameObject) : Event(true), _gameObject(gameObject) {}
		~EventGameObjectEnabled() override = default;

		[[nodiscard]]
		Ref<GameObject> GetGameObject() const { return _gameObject; }

		EVENT_CLASS_TYPE(GameObjectEnabled)
	private:
		Ref<GameObject> _gameObject;
	};

	class EventGameObjectDisabled final : public Event
	{
	public:
		EventGameObjectDisabled(Ref<GameObject> gameObject) : Event(true), _gameObject(gameObject) {}
		~EventGameObjectDisabled() override = default;

		[[nodiscard]]
		Ref<GameObject> GetGameObject() const { return _gameObject; }

		EVENT_CLASS_TYPE(GameObjectDisabled)
	private:
		Ref<GameObject> _gameObject;
	};
}
