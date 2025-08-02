#include "stdafx.h"

#include "Project.h"

#include "Setting/ProjectSetting.h"

namespace DentyEngine
{
	const FilePath Project::DEFAULT_PROJECT_FILE_PATH = FilePath("Project\\ChickenKart.DentyProjectSetting");

	Ref<ProjectSetting> Project::_setting = nullptr;

	Project::Project()
	{
	}

	void Project::OnLaunch()
	{
		if (DEFAULT_PROJECT_FILE_PATH.IsValid())
		{
			Load(DEFAULT_PROJECT_FILE_PATH);
		}
		else
		{
			Save(DEFAULT_PROJECT_FILE_PATH);
		}
	}

	void Project::Save(const FilePath& filePath)
	{
		const auto setting = std::make_shared<ProjectSetting>();
		setting->SetFilePath(filePath);

		setting->Save(false);

		_setting = setting;
	}

	void Project::Load(const FilePath& filePath)
	{
		const auto setting = std::make_shared<ProjectSetting>();
		setting->Load(filePath.Get());

		_setting = std::make_shared<ProjectSetting>();
		setting->Clone(_setting);
	}

	Ref<ProjectSetting> Project::GetProjectSetting()
	{
		return _setting;
	}
}
