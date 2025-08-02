#pragma once

#include "System/Core/DFilePath.h"

#include <utils/mono-forward.h>

#include <FileWatch.hpp>

extern "C"
{
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
}

// Reference
// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Scripting/ScriptEngine.h
// https://nilssondev.com/mono-guide/book/introduction/about-mono.html

namespace DentyEngine
{
	class EventOnApplicationStateChangedToPlay;
	class EventOnApplicationStateChangedToEdit;
	class EventRuntimeSceneChanged;
	class EventRuntimeSceneChange;
	class Script;
	class EventOnComponentAdded;
	class ScriptComponent;
	class Scene;

	class Event;

	class EventOnSceneActivated;
	class EventOnSceneDeActivated;

	class EventGameObjectEnabled;
	class EventGameObjectDisabled;

	struct ScriptEngineData
    {
		ScriptEngineData() : rootDomain(), appDomain(),
			coreAssembly(), coreAssemblyImage(),
    		appAssembly(), appAssemblyImage(), appAssemblyFileWatcher(),
			coreAssemblyFilePath(), appAssemblyFilePath(),
			scripts(), activeScene(), enableDebugging(true), requestedReload() {}

		//
		// Mono
		//
		MonoDomain* rootDomain;

		// Usually use this domain instead of _rootDomain.
		MonoDomain* appDomain;

		MonoAssembly* coreAssembly;
		MonoImage* coreAssemblyImage;

		MonoAssembly* appAssembly;
		MonoImage* appAssemblyImage;

    	Scope<filewatch::FileWatch<std::string>> appAssemblyFileWatcher;

		FilePath coreAssemblyFilePath;
		FilePath appAssemblyFilePath;

		//
		// Engine
		//
		std::vector<Ref<ScriptComponent>> scripts;

		Ref<Scene> activeScene;

		bool enableDebugging;
		bool requestedReload;
    };

	class ScriptEngine final
	{
	public:
		ScriptEngine();
		~ScriptEngine() = default;

		static void Initialize();

		static void OnUpdate();
		static void OnFixedUpdate();
		static void OnLateUpdate();
		static void OnEnabled(EventGameObjectEnabled* e);
		static void OnDisabled(EventGameObjectDisabled* e);
		static void OnGui();

		static void OnRequestedReload();

		static void Finalize();

		static void ReloadAssembly();

		static void OnEvent(Event* e);

		static void OnReceivedScript(const Ref<ScriptComponent>& script);

		static void OnScenePlayButtonPressed();

		static void Add(Ref<ScriptComponent> script);
		static void Remove(const Ref<ScriptComponent>& script);

		//
		// Getter
		//

		//
		// Mono
		//
		[[nodiscard]]
		static MonoDomain* GetAppDomain() { return _data.appDomain; }

		[[nodiscard]]
		static MonoImage* GetCoreAssemblyImage() { return _data.coreAssemblyImage; }

		[[nodiscard]]
		static MonoImage* GetAppAssemblyImage() { return _data.appAssemblyImage; }

		//
		// Engine
		//

		[[nodiscard]]
		static Ref<Scene> GetActiveScene() { return _data.activeScene; }

		[[nodiscard]]
		static bool HasActiveScene() { return (_data.activeScene != nullptr); }

		[[nodiscard]]
		static bool IsRequestedReload() { return _data.requestedReload; }
	public:
		static void OnAppAssemblyFileSystemEvent(const std::string& filePath, const filewatch::Event changeType);

		//
		// Handle exception functions.
		//
		static void OnHandleReferenceNullException(const Ref<ScriptComponent>& script);

		static void GenerateCSharpFile(const char* filePath);
	public:
		static const FilePath SCRIPT_SAVE_FOLDER_PATH;

	private:
		static void InitializeMono();
		static void FinalizeMono();

		static void LoadAssemblies();

		static void RecreateScripts();

		static void OnSceneActivated(EventOnSceneActivated* e);
		static void OnSceneDeActivated(EventOnSceneDeActivated* e);
		static void OnRuntimeSceneChange(EventRuntimeSceneChange* e);
		static void OnRuntimeSceneChanged(EventRuntimeSceneChanged* e);
		static void OnApplicationStateChangedToPlay(EventOnApplicationStateChangedToPlay* e);
		static void OnApplicationStateChangedToEdit(EventOnApplicationStateChangedToEdit* e);
	private:
		static ScriptEngineData _data;
	private:
		static const FilePath CORE_ASSEMBLY_FILE_PATH;
		static const FilePath APP_ASSEMBLY_FILE_PATH;
	};
}