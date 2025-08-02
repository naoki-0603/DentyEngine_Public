#include "stdafx.h"

#include "ProjectSetting.h"

#include "System/Serializer/ProjectSettingSerializer.h"

namespace DentyEngine
{
	RuntimeSceneChange::RuntimeSceneChange() :
		scenes()
	{
	}

	void RuntimeSceneChange::Reset()
	{
		scenes.clear();
	}

	ProjectSetting::ProjectSetting() :
		Setting(), runtimeSceneChange()
	{
		name = "ProjectSetting";
	}

	ProjectSetting::ProjectSetting(const ProjectSetting& source) :
		Setting(source), runtimeSceneChange(source.runtimeSceneChange)
	{
	}

	void ProjectSetting::OnGui()
	{
	}

	void ProjectSetting::Save(bool isUpdate) const
	{
		// If failed
		if (!ProjectSettingSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " project setting!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " project setting!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " project setting!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " project setting!";
		}
	}

	void ProjectSetting::Load(const std::filesystem::path& filePath)
	{
		auto setting = std::make_shared<ProjectSetting>();

		// If failed
		if (!ProjectSettingSerializer::DeserializeFromJson(filePath, setting))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " setting!";

			return;
		}

		setting->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " setting!";
	}

	void ProjectSetting::Clone(const Ref<Setting>& setting)
	{
		DENTY_CHECK_CONDITION(setting->GetType() == std::string("ProjectSetting"))

		Setting::Clone(setting);

		auto&& target = std::static_pointer_cast<ProjectSetting>(setting);
		target->runtimeSceneChange = runtimeSceneChange;
	}
}
