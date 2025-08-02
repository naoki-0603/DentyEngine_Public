#pragma once

#include "Setting.h"

#include "System/Core/DString.h"

#include "System/Physics/CollisionFilter.h"

namespace DentyEngine
{
    class CollisionFilterSetting final : public Setting, public std::enable_shared_from_this<CollisionFilterSetting>
    {
    public:
        CollisionFilterSetting();
        CollisionFilterSetting(const CollisionFilterSetting& source);
        ~CollisionFilterSetting() override = default;

        void Initialize();

        void Save(bool isUpdate) const override;
        void Load(const std::filesystem::path& filePath) override;

        void OnGui() override;

        void Clone(const Ref<Setting>& setting) override;

        void OnUpdate();

        void SelectCollisionFilterGui(_Out_ CollisionFilter* filter) const;

        //
        // Getter
        //
        [[nodiscard]]
        std::optional<std::string> GetNameAt(uint16_t group) const;

        [[nodiscard]]
        std::optional<uint32_t> GetGroupBitAt(const std::string& name) const;

        [[nodiscard]]
        const char* GetAddFileNameAtEnd() const override { return "_CollisionFilter"; }

        [[nodiscard]]
        const char* GetType() const override { return "CollisionFilter"; }
    public:
        static constexpr uint32_t MAX_FILTER_NUM = 32u;
        static constexpr Vector2 BUTTON_SIZE = { 8.0f, 8.0f };

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
               archive(
                    cereal::base_class<Setting>(this),
                    CEREAL_NVP(_filters)
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
                    CEREAL_NVP(_filters)
                ); 
            }
        }

    private:
        std::map<std::string, CollisionFilter> _filters;
        std::set<std::string> _filterNames;
        std::vector<std::string> _filterNamesForComboBox;
    };

    //
    // Macros.
    //
#define DENTY_DEFINE_COLLISION_FILTER_GUI(name, flags, filterMap) \
    do \
    { \
        std::string filterName = { }; \
        ImGui::Text(name); \
        ImGui::SameLine(); \
        ImGui::Spacing(); \
        String::InputTextGui((name), &(filterName), flags); \
        (filterMap).try_emplace(filterName); \
    } while(false);

#define DENTY_DEFINE_GROUP_RADIO_BUTTON(name, groupPointer, isLastLineBool) \
    ImGui::RadioButton(name, groupPointer); \
    if (not (isLastLineBool)) { ImGui::SameLine(); }

}

CEREAL_CLASS_VERSION(DentyEngine::CollisionFilterSetting, 1u)