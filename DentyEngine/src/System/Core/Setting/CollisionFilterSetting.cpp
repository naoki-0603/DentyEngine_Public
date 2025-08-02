#include "stdafx.h"

#include "CollisionFilterSetting.h"

#include "System/Serializer/SettingFileSerializer.h"

#include "Utilities/ImGuiUtilities.h"

namespace DentyEngine
{
    CollisionFilterSetting::CollisionFilterSetting() :
        Setting(), _filters(), _filterNames(), _filterNamesForComboBox()
    {
        name = "CollisionFilterSetting";
    }

    CollisionFilterSetting::CollisionFilterSetting(const CollisionFilterSetting& source) :
        Setting(source), _filters(source._filters), _filterNames(source._filterNames), _filterNamesForComboBox(source._filterNamesForComboBox)
    {
    }

    void CollisionFilterSetting::Initialize()
    {

    }

    void CollisionFilterSetting::Save(bool isUpdate) const
    {
        // If failed.
		if (!SettingFileSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
		{
			DENTY_SET_ERR_COLOR

			if (isUpdate)
			{
				DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " collision filter setting!";
			}
			else
			{
				DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " collision filter setting!";
			}

			return;
		}

		DENTY_SET_INFO_COLOR

		if (isUpdate)
		{
			DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " collision filter setting!";
		}
		else
		{
			DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " collision filter setting!";
		}
    }

    void CollisionFilterSetting::Load(const std::filesystem::path& filePath)
    {
        auto collisionFilterSetting = std::make_shared<CollisionFilterSetting>();

		// If failed.
		if (!SettingFileSerializer::DeserializeFromJson(filePath, collisionFilterSetting))
		{
			DENTY_SET_ERR_COLOR
			DENTY_ERR_LOG << "Failed to load " << filePath << " collision filter setting!";

			return;
		}

		collisionFilterSetting->Clone(shared_from_this());

		DENTY_SET_INFO_COLOR
		DENTY_INFO_LOG << "Succeed to load " << filePath << " collision filter setting!";
    }

    void CollisionFilterSetting::OnGui()
    {
        if (ImGui::CollapsingHeader("CollisionFilterSetting##EditorSettingWindow", ImGuiTreeNodeFlags_DefaultOpen))
        {
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 1",  0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 2",  0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 3",  0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 4",  0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 5",  0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 6",  0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 7",  0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 8",  0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 9",  0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 10", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 11", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 12", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 13", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 14", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 15", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 16", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 17", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 18", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 19", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 20", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 21", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 22", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 23", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 24", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 25", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 26", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 27", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 28", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 29", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 30", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 31", 0, _filters)
            DENTY_DEFINE_COLLISION_FILTER_GUI("Filter 32", 0, _filters)
        }
    }

    void CollisionFilterSetting::Clone(const Ref<Setting>& setting)
    {
        DENTY_CHECK_CONDITION(setting->GetType() == std::string("CollisionFilter"))

        auto&& target = std::static_pointer_cast<CollisionFilterSetting>(setting);

        // Base class
        {
            Setting::Clone(target);
        }

        // This class
        {
            target->_filters = _filters;
            target->_filterNamesForComboBox = _filterNamesForComboBox;
            target->_filterNames = _filterNames;
        }
    }

    void CollisionFilterSetting::OnUpdate()
    {
        _filterNames.clear();
        _filterNamesForComboBox.clear();

        // Store filter names.
        {
            for (const std::string& filter : _filters | std::views::keys)
            {
                _filterNames.emplace(filter);
                _filterNamesForComboBox.emplace_back(filter);
            }
        }
    }

    void CollisionFilterSetting::SelectCollisionFilterGui(CollisionFilter* filter) const
    {
        DENTY_CHECK_STL_SIZE(_filterNamesForComboBox, 32u)

        // If filter changed.
        if (not GuiUtils::ComboBoxGui("##SelectCollisionFilterGui",
            _filterNamesForComboBox.data(), _filterNamesForComboBox.size(),
            filter->name))
        {
            DENTY_CHECK_STL_MAP_VALUE_EXISTS(_filters, filter->name)

            _filters.find(filter->name)->second;
        }
    }

    std::optional<std::string> CollisionFilterSetting::GetNameAt(uint16_t group) const
    {
        uint32_t counter = 0u;
        for (const auto& filterName : _filters | std::views::keys)
        {
            if (group != counter)
            {
                ++counter;

                continue;
            }

            return filterName;
        }

        return std::nullopt;
    }

    std::optional<uint32_t> CollisionFilterSetting::GetGroupBitAt(const std::string& name) const
    {
        /*auto&& it = _filters.find(name);
        if (it != _filters.end())
        {
            return it->second;
        }*/

        DENTY_NOT_IMPLEMENTED_FUNCTION(__func__)

        return std::nullopt;
    }
}
