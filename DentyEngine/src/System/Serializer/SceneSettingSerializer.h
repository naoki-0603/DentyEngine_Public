#pragma once

namespace DentyEngine
{
	class SceneSetting;

	class SceneSettingSerializer final
	{
	public:
		SceneSettingSerializer();
		~SceneSettingSerializer() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(SceneSettingSerializer)

		[[nodiscard]]
		static bool SerializeAsJson(const std::filesystem::path&, const Ref<const SceneSetting>& setting);

		[[nodiscard]]
		static bool DeserializeFromJson(const std::filesystem::path&, Ref<SceneSetting>& setting);
	public:
		static const std::string SCENE_SETTING_FILE_OUTPUT_PATH;
	};
}