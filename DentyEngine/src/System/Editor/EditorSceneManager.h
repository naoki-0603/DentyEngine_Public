#pragma once

#include "System/Core/DFilePath.h"

namespace DentyEngine
{
	class Event;
	class Scene;

	class EventRuntimeSceneChange;

	struct EditorSceneChangeParameters
	{
	public:
		EditorSceneChangeParameters();

		void Reset();
	public:
		bool isChangeToNewScene;
	};

	struct RuntimeSceneChangeParameters
	{
	public:
		RuntimeSceneChangeParameters();

		void Reset();
	public:
		FilePath beginPlaySceneFilePath;

		bool runtimeSceneChange;
		bool reloadScene;
	};

	struct CommonSceneChangeParameters
	{
	public:
		CommonSceneChangeParameters();

	public:
		Ref<Scene> currentScene;

		FilePath reservedLoadSceneFilePath;
	};

	class EditorSceneManager final
	{
	public:
		EditorSceneManager();
		~EditorSceneManager() = default;

		static void OnGui();
		static void OnEvent(Event* e);

		// If contain reserved load scene file path.
		// load scene on next frame.
		static void ChangeScene(const FilePath& filePath);


		//
		// Setter
		//
		static void SetReservedSceneFilePath(const FilePath& filePath, bool isNewScene, bool runtimeSceneChange, bool runtimeReloadScene);

		static void SetCurrentPlayingScene(Ref<Scene> playingScene);
		static void SetBeginPlaySceneFilePath(const FilePath& filePath);

		//
		// Getter
		//
		[[nodiscard]]
		static Ref<Scene> GetCurrentPlayingScene();

		[[nodiscard]]
		static const FilePath& GetBeginPlaySceneFilePath();

		[[nodiscard]]
		static Ref<Scene> GetActiveScene();

		[[nodiscard]]
		static const FilePath& GetReservedSceneFilePath();

		[[nodiscard]]
		static bool HasReservedSceneFilePath();

		[[nodiscard]]
		static bool HasActiveScene();
	private:
		static void OnRuntimeSceneChange(EventRuntimeSceneChange* e);

	private:
		static EditorSceneChangeParameters _editorSceneChangeParameters;
		static RuntimeSceneChangeParameters _runtimeSceneChangeParameters;
		static CommonSceneChangeParameters _commonSceneChangeParameters;
	};
}
