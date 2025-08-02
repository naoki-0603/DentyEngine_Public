#pragma once

#include "DFilePath.h"

namespace DentyEngine
{
	class Scene;

	class RuntimeSceneManager final
	{
	public:
		RuntimeSceneManager();
		~RuntimeSceneManager() = default;

		static void OnLaunch();

		static void OnGui();

		static void Add(Ref<Scene> scene);
		static void Remove(Ref<Scene> scene);

		static void ChangeSceneFromIndex(int32_t index, bool reloadScene);
		static void ChangeSceneFromName(std::string_view name, bool reloadScene);

		[[nodiscard]]
		static int32_t GetSceneCount();
	private:
		static std::unordered_map<FilePath, Ref<Scene>, FilePath::Hash> _scenes;
	};
}
