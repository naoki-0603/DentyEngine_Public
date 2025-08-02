#pragma once

#include "System/Events/Event.h"
#include "System/Events/EventConstants.h"

namespace DentyEngine
{
	class EventLayerInitialized final : public Event
	{
	public:
		EventLayerInitialized() : Event(true) {}
		~EventLayerInitialized() override = default;

		EVENT_CLASS_TYPE(LayerInitialized)
	};

	class EventBeforeScriptAssemblyReload final : public Event
	{
	public:
		EventBeforeScriptAssemblyReload() : Event(true) {}
		~EventBeforeScriptAssemblyReload() override = default;

		EVENT_CLASS_TYPE(BeforeScriptAssemblyReload)
	};

	class EventScriptAssemblyReloaded final : public Event
	{
	public:
		EventScriptAssemblyReloaded() : Event(true) {}
		~EventScriptAssemblyReloaded() override = default;

		EVENT_CLASS_TYPE(ScriptAssemblyReloaded)
	};

	class EventOnApplicationStateChangedToEdit final : public Event
	{
	public:
		EventOnApplicationStateChangedToEdit() : Event(true) {}
		~EventOnApplicationStateChangedToEdit() override = default;

		EVENT_CLASS_TYPE(OnApplicationStateChangedToEdit)
	};

	class EventOnApplicationStateChangedToPlay final : public Event
	{
	public:
		EventOnApplicationStateChangedToPlay() : Event(true) {}
		~EventOnApplicationStateChangedToPlay() override = default;

		EVENT_CLASS_TYPE(OnApplicationStateChangedToPlay)
	};

	class EventOnApplicationStateChangedToPause final : public Event
	{
	public:
		EventOnApplicationStateChangedToPause() : Event(true) {}
		~EventOnApplicationStateChangedToPause() override = default;

		EVENT_CLASS_TYPE(OnApplicationStateChangedToPause)
	};

	class EventRuntimeSceneChange final : public Event
	{
	public:
		EventRuntimeSceneChange(std::string_view filePath, bool reloadScene) : Event(true), _filePath(filePath), _reloadScene(reloadScene) {}
		~EventRuntimeSceneChange() override = default;

		[[nodiscard]]
		const std::string& GetFilePath() const { return _filePath; }

		[[nodiscard]]
		bool IsReloadScene() const { return _reloadScene; }

		EVENT_CLASS_TYPE(RuntimeSceneChange)
	private:
		std::string _filePath;

		bool _reloadScene;
	};

	class EventRuntimeSceneChanged final : public Event
	{
	public:
		EventRuntimeSceneChanged(std::string_view sceneName) : Event(true), _sceneName(sceneName) {}
		~EventRuntimeSceneChanged() override = default;

		[[nodiscard]]
		const std::string& GetSceneName() const { return _sceneName; }

		EVENT_CLASS_TYPE(RuntimeSceneChanged)
	private:
		std::string _sceneName;
	};
}