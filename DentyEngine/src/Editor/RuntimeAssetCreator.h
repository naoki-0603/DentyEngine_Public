#pragma once

#include "System/Core/DFilePath.h"
#include "System/Resource/Asset.h"

namespace DentyEngine
{
    class Material;
    class Shader;

    class AnimatorController;

    class Scene;

    class Script;

    struct AssetInputName
    {
        AssetInputName() : targetAsset(), newName(), currentDirectory(), isEditing() {}

        void Reset()
        {
            targetAsset = nullptr;
            newName = {};
            currentDirectory = {};

            isEditing = false;
        }

        Ref<Asset> targetAsset;

        std::string newName;
        std::string currentDirectory;

        bool isEditing;
    };

    class RuntimeAssetCreator final
    {
    public:
        RuntimeAssetCreator();
        ~RuntimeAssetCreator() = default;

        static Ref<Asset> OnInputFileName();

        [[nodiscard]]
        static std::optional<Ref<Material>> CreateMaterial(const FilePath&, Ref<Shader>);

        [[nodiscard]]
        static std::optional<Ref<AnimatorController>> CreateAnimatorController(const FilePath&);

        [[nodiscard]]
        static std::optional<Ref<Scene>> CreateScene(const FilePath&);

        static void CreateScript(const FilePath& directory);

        [[nodiscard]]
        static bool IsInputFileName() { return _assetInputName.isEditing; }
    private:
        [[nodiscard]]
        static Ref<Script> InternalCreateScript(); 

        [[nodiscard]]
        static bool MakeUniqueName();

    private:
        static AssetInputName _assetInputName;
    };
}