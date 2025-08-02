#include "stdafx.h"

#include "RuntimeAssetCreator.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "Graphics/Renderer/Material.h"

#include "System/Resource/Animations/AnimatorController.h"

#include "System/Scene/Scene.h"

#include "System/Serializer/SerializeExtensions.h"

#include <format>

#include "System/Resource/Script.h"

#include "System/Core/DString.h"

namespace DentyEngine
{
    AssetInputName RuntimeAssetCreator::_assetInputName = {};

    RuntimeAssetCreator::RuntimeAssetCreator()
    {
    }

    Ref<Asset> RuntimeAssetCreator::OnInputFileName()
    {
        if (_assetInputName.isEditing)
        {
            ImGui::OpenPopup("##EditingParameterNamePopup");

            using AssetType = Asset::AssetType;
            if (ImGui::BeginPopup("##EditingParameterNamePopup"))
            {
                const AssetType assetType = _assetInputName.targetAsset->GetAssetType();
                const std::string assetTypeString = std::string(NAMEOF_ENUM_FLAG(assetType));
                const std::string text = std::format(
                    "{} - Filename",
                    assetTypeString.c_str()
                );

                ImGui::Text(text.c_str());

                String::InputTextGui("##EditingParameterName", &_assetInputName.newName);

                const bool isMouseClickedOnWindow = (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsItemClicked());
                if (isMouseClickedOnWindow)
                {
                    ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();

                    _assetInputName.Reset();

                    return nullptr;
                }

                if (ImGui::IsKeyDown(ImGuiKey_Enter))
                {
                    ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();

                    // If could not create unique name, return.
                    if (not MakeUniqueName())
                    {
                        _assetInputName.Reset();

                        return nullptr;
                    }

                    Ref<Asset> asset = nullptr;
                    switch (assetType)
                    {
                    case AssetType::Material:
                        {
                            
                        }
                        break;
                    case AssetType::Controller:
                        {
                            
                        }
                        break;
                    case AssetType::Scene:
                        {
                            
                        }
                        break;
                    case AssetType::Script:
                        {
                            asset = InternalCreateScript();
                        }
                        break;
                    }

                    _assetInputName.Reset();

                    return asset;
                }

                ImGui::EndPopup();
            }
        }

        return nullptr;
    }

    std::optional<Ref<Material>> RuntimeAssetCreator::CreateMaterial(const FilePath& directory, Ref<Shader> shader)
    {
        // Create unique material name.
        const auto uniqueName = EditorContentBrowser::MakeUniqueNameInDirectory(directory, "NewMaterial", Asset::AssetType::Material);
        if (!uniqueName.has_value())
        {
            DENTY_SET_ERR_COLOR
            DENTY_ERR_LOG << "Couldn't make unique name!";

            return std::nullopt;
        }

        // Create path.
        FilePath path = directory;
        path += uniqueName.value();
        path += MATERIAL_FILE_EXTENSION;

        // Create material
        const auto material = Utils::CreateRef<Material>();
        //material->Create(shader, uniqueName.value(), path.Get(), true);

        return material;
    }

    std::optional<Ref<AnimatorController>> RuntimeAssetCreator::CreateAnimatorController(const FilePath& directory)
    {
        // Create unique controller name.
        const auto uniqueName = EditorContentBrowser::MakeUniqueNameInDirectory(directory, "NewAnimatorController", Asset::AssetType::Controller);
        if (!uniqueName.has_value())
        {
            DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Couldn't make unique name!";

            return std::nullopt;
        }

        // Create path.
        FilePath path = directory;
        path += uniqueName.value();

        // Create controller
        const auto controller = std::make_shared<AnimatorController>();
        controller->Create(uniqueName.value(), path.Get());

        return controller;
    }

    std::optional<Ref<Scene>> RuntimeAssetCreator::CreateScene(const FilePath& directory)
    {
        // Create unique scene name.
        const auto uniqueName = EditorContentBrowser::MakeUniqueNameInDirectory(directory, "NewScene", Asset::AssetType::Scene);
        if (!uniqueName.has_value())
        {
            DENTY_SET_ERR_COLOR
                DENTY_ERR_LOG << "Couldn't make unique name!";

            return std::nullopt;
        }

        // Create path.
        FilePath path = directory;
        path += uniqueName.value();
        path += SCENE_FILE_EXTENSION;

        // Create scene
        const auto scene = std::make_shared<Scene>(uniqueName.value());
        scene->SetFilePath(path.GetAsString());
        scene->SetFilePathToSerialized(path.GetAsString());

        return scene;
    }

    void RuntimeAssetCreator::CreateScript(const FilePath& directory)
    {
        _assetInputName.newName = {};
        _assetInputName.currentDirectory = directory.GetAsString();
        _assetInputName.targetAsset = std::make_shared<Script>();
        _assetInputName.isEditing = true;

        //const auto uniqueName = EditorContentBrowser::MakeUniqueNameInDirectory(directory, "", Asset::AssetType::Scene);
        //if (!uniqueName.has_value())
        //{
        //    DENTY_SET_ERR_COLOR
        //        DENTY_ERR_LOG << "Couldn't make unique name!";

        //    return std::nullopt;
        //}

        //// Create path.
        //FilePath path = directory;
        //path += uniqueName.value();
        //path += SCENE_FILE_EXTENSION;

        //// Create scene
        //const auto scene = std::make_shared<Scene>(uniqueName.value());
        //scene->SetFilePath(path.GetAsString());
        //scene->SetFilePathToSerialized(path.GetAsString());
    }

    Ref<Script> RuntimeAssetCreator::InternalCreateScript()
    {
        // Create file path.
        FilePath filePath = { };
        {
            filePath += _assetInputName.currentDirectory;
            filePath.ReplaceFilename(_assetInputName.newName);
        }

        // Create script
        const Ref<Script> script = std::static_pointer_cast<Script>(_assetInputName.targetAsset);
        {
            script->Create(filePath);
        }

        DENTY_SET_INFO_COLOR
            DENTY_INFO_LOG << "Succeed to create script asset!";

        return script;
    }

    bool RuntimeAssetCreator::MakeUniqueName()
    {
        const std::optional<std::string> uniqueName = EditorContentBrowser::MakeUniqueNameInDirectory(
            _assetInputName.currentDirectory,
            _assetInputName.newName,
            _assetInputName.targetAsset->GetAssetType()
        );

        if (not uniqueName.has_value())
        {
            return false;
        }

        _assetInputName.newName = uniqueName.value();

        return true;
    }
}
