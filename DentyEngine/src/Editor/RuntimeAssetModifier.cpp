#include "stdafx.h"

#include "RuntimeAssetModifier.h"

#include "System/Resource/Asset.h"

namespace DentyEngine
{
    RuntimeAssetModifier::RuntimeAssetModifier()
    {
    }

    bool RuntimeAssetModifier::Rename(std::string_view oldName, std::string_view newName, const Ref<Asset>& asset)
    {
        asset->Rename(newName);

        DENTY_SET_INFO_COLOR
        DENTY_INFO_LOG << "Rename from " << oldName << " to " << newName;

        return true;
    }

    bool RuntimeAssetModifier::Delete(const Ref<Asset>& asset)
    {
        asset->GetFilePathToSerialized().RemoveFile();

        DENTY_SET_INFO_COLOR
        DENTY_INFO_LOG << "Delete asset " << asset->GetFilePathToSerialized();

        return true;
    }
}
