#pragma once

#include "System/Core/DFilePath.h"

namespace DentyEngine
{
    class Setting
    {
    public:
        Setting();
        Setting(const Setting& source);
        virtual ~Setting() = default;

        virtual void Save(bool isUpdate = false) const = 0;
        virtual void Load(const std::filesystem::path& filePath) = 0;

        virtual void OnGui() = 0;

        virtual void Clone(const Ref<Setting>& setting);

        //
        // Setter
        //
        void SetFilePath(const FilePath& filePath);

        //
        // Getter
        //
        [[nodiscard]]
        virtual const char* GetAddFileNameAtEnd() const = 0;

        [[nodiscard]]
        virtual const char* GetType() const = 0;

        [[nodiscard]]
        const FilePath& GetFilePathToSerialized() const { return _filePathToSerialized; }
    public:
        std::string name;

    protected:
        FilePath _filePathToSerialized;

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(name),
                    CEREAL_NVP(_filePathToSerialized)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    CEREAL_NVP(name),
                    CEREAL_NVP(_filePathToSerialized)
                );
            }
        }
    
    };
}

CEREAL_CLASS_VERSION(DentyEngine::Setting, 1u)