#pragma once

#include "Setting.h"

namespace DentyEngine
{
    struct RuntimeSceneChange
    {
    public:
        RuntimeSceneChange();
        ~RuntimeSceneChange() = default;

        void Reset();
    public:
        std::unordered_map<std::string, FilePath> scenes;

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(scenes)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if(version <= 1u)
            {
                archive(
                    CEREAL_NVP(scenes)
                );
            }
        }
    };

	class ProjectSetting final : public Setting, public std::enable_shared_from_this<ProjectSetting>
	{
	public:
        ProjectSetting();
        ProjectSetting(const ProjectSetting& source);
        ~ProjectSetting() override = default;

        void OnGui() override;

        void Save(bool isUpdate) const override;
        void Load(const std::filesystem::path& filePath) override;

        void Clone(const Ref<Setting>& setting) override;

        [[nodiscard]]
        const char* GetAddFileNameAtEnd() const override { return "_ProjectSetting"; }

        [[nodiscard]]
        const char* GetType() const override { return "ProjectSetting"; }
    public:
        RuntimeSceneChange runtimeSceneChange;

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Setting>(this),
                    CEREAL_NVP(runtimeSceneChange)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Setting>(this),
                    CEREAL_NVP(runtimeSceneChange)
                );
            }
        }
	};
}

CEREAL_CLASS_VERSION(DentyEngine::RuntimeSceneChange, 1u)
CEREAL_CLASS_VERSION(DentyEngine::ProjectSetting, 1u)