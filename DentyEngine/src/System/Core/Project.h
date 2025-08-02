#pragma once

#include "DFilePath.h"

namespace DentyEngine
{
	class ProjectSetting;

	class Project
	{
	public:
		Project();
		~Project() = default;

		static void OnLaunch();

		static void Save(const FilePath& filePath);
		static void Load(const FilePath& filePath);

		//
		// Getter
		//
		[[nodiscard]]
		static Ref<ProjectSetting> GetProjectSetting();
	private:
		static Ref<ProjectSetting> _setting;

	private:
		static const FilePath DEFAULT_PROJECT_FILE_PATH;
	};
}
