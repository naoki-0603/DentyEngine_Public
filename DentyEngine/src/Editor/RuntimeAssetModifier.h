#pragma once

namespace DentyEngine
{
    class Asset;

    class RuntimeAssetModifier final
    {
    public:
        RuntimeAssetModifier();
        ~RuntimeAssetModifier() = default;

        [[nodiscard]]
        static bool Rename(std::string_view oldName, std::string_view newName, const Ref<Asset>& asset);

        [[nodiscard]]
        static bool Delete(const Ref<Asset>& asset);
    };
}