#pragma once

namespace DentyEngine
{
	class ProjectSetting;

	class ProjectSettingSerializer final
	{
	public:
		ProjectSettingSerializer();
		~ProjectSettingSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ProjectSettingSerializer)

		[[nodiscard]]
		static bool SerializeAsJson(const std::filesystem::path&, const Ref<const ProjectSetting>& projectSetting);

		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<ProjectSetting>& projectSetting);
	};
}