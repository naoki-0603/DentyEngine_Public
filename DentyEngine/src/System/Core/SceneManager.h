#pragma once

#include "DFilePath.h"

namespace DentyEngine
{
	class Scene;
	class SceneManager
	{
	public:
		SceneManager();
		virtual ~SceneManager() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SceneManager)

		virtual void ChangeScene(const FilePath& filePath) = 0;

		//
		// Getter
		//
		[[nodiscard]]
		Ref<Scene> GetActiveScene() const { return _activeScene; }

		[[nodiscard]]
		bool HasActiveScene() const { return (_activeScene != nullptr); }
	protected:
		Ref<Scene> _activeScene;
	};
}