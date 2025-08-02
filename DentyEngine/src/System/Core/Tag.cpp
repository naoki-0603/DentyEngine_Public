#include "stdafx.h"

#include "Tag.h"

namespace DentyEngine
{
    const std::array<std::string, 12> Tag::TAGS =
	{
		"UnTagged", "MainCamera", "Player", "Stage", "StageProp", "CameraTarget", "CheckPoint", "Canvas", "UI",
		"Coin", "Grass", "InvisibleWall"
	};

	Tag::Tag(const std::string& tag) :
		tag(tag)
	{
	}

    std::optional<uint32_t> Tag::GetTagNumberFrom(std::string_view tag)
    {
		for (uint32_t index = 0u; index < static_cast<uint32_t>(TAGS.size()); ++index)
		{
		    if (TAGS.at(index) == tag)
				return index;
		}

		return std::nullopt;
    }
}
