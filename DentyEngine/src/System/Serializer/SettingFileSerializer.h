#pragma once

namespace DentyEngine
{
    class SceneSetting;
    class CollisionFilterSetting;

    class SettingFileSerializer final
    {
    public:
        SettingFileSerializer();
        ~SettingFileSerializer() = default;

        DELETE_COPY_MOVE_CONSTRUCTORS(SettingFileSerializer)

        [[nodiscard]]
        static bool SerializeAsJson(const std::filesystem::path& filePath, const Ref<const SceneSetting>& setting);

        [[nodiscard]]
        static bool SerializeAsJson(const std::filesystem::path& filePath, const Ref<const CollisionFilterSetting>& setting);

        [[nodiscard]]
        static bool DeserializeFromJson(const std::filesystem::path& filePath, Ref<SceneSetting>& setting);

        [[nodiscard]]
        static bool DeserializeFromJson(const std::filesystem::path& filePath, Ref<CollisionFilterSetting>& setting);
    public:
        static const std::string OUTPUT_FILE_PATH;
    };
}
