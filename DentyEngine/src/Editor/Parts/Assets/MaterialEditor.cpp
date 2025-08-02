#include "stdafx.h"

#include "MaterialEditor.h"

#include "Editor/EditorHelper.h"
#include "Utilities/ImGuiUtilities.h"

#include "Graphics/Renderer/Material.h"
#include "Graphics/Renderer/Texture2D.h"

#include "System/Editor/DragAndDrop/DragAndDrop.h"

#include "System/Events/EventEditor.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/TextureLibrary.h"

namespace DentyEngine
{
    const Vector2 MaterialEditor::BUTTON_SIZE = { 16, 16 };
    const Vector2 MaterialEditor::THUMBNAIL_SIZE = { 64, 64 };
    const Vector2 MaterialEditor::CROSS_MARK_SIZE = { 16, 16 };

    MaterialEditor::MaterialEditor() :
        IEditor(), AssetEditor(), _oldMaterial(), _material(), _textureEmptyRef(), _textureXMarkRef(), _renderingMode(),
		_selectTexture(MaterialEditorSelectTexture::None), _isOpenSelectTextureWindow()
    {
        const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

        _textureEmptyRef = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::TEXTURE_EMPTY_FILE_PATH).value();
        _textureXMarkRef = textureLibrary->Find<Texture2D>(TextureLibraryCommonPaths::COMMON_X_MARK_FILE_PATH).value();
    }

    void MaterialEditor::OnEvent(Event* e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<EventOnMaterialEditorOpened>(BIND_EVENT_FUNCTION(OnOpened));
        dispatcher.Dispatch<EventOnMaterialEditorClosed>(BIND_EVENT_FUNCTION(OnClosed));
    }

    void MaterialEditor::OnGui(Ref<Scene> activeScene)
    {
        if (not _open) return;

        if (ImGui::Begin("MaterialEditor##AssetEditor", &_open, IMGUI_DEFAULT_WINDOW_FLAGS))
        {
            const ImVec2 windowSize = ImGui::GetWindowSize();

            // Rendering mode.
            {
                ImGui::PushTextWrapPos(windowSize.x * 0.5f);
                ImGui::TextWrapped("RenderingMode");
                ImGui::PopTextWrapPos();

                ImGui::SameLine();

                ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.4f);
            	if (!GuiUtils::ComboBoxGui(
					"##RenderingModeInMaterialEditor",
                    Material::RENDERING_MODES.data(), static_cast<int32_t>(Material::RENDERING_MODES.size()), _renderingMode))
                {
	                if (_renderingMode == std::string(NAMEOF_ENUM(RenderingMode::Opaque)))
	                {
                        _material->parameters.renderingMode = RenderingMode::Opaque;
	                }
                    else if (_renderingMode == std::string(NAMEOF_ENUM(RenderingMode::Transparent)))
                    {
                        _material->parameters.renderingMode = RenderingMode::Transparent;
                    }
                }
            }

            // Influence hemi-sphere light.
            {
                ImGui::PushTextWrapPos(windowSize.x * 0.5f);
                ImGui::TextWrapped("InfluenceHemisphereLight");
                ImGui::PopTextWrapPos();

                ImGui::SameLine();

                ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.5f);
                ImGui::Checkbox("##InfluenceHemisphereLight_MaterialEditor", &_material->parameters.influenceHemisphereLight);
            }

            ImGui::Spacing();
            ImGui::PushTextWrapPos(windowSize.x * 0.5f);
            ImGui::TextWrapped("Albedo");
            ImGui::PopTextWrapPos();

            // Albedo
            {
                const bool hasAlbedoMap = _material->HasTextureAt(MaterialTextureIndex::Albedo);
                if (hasAlbedoMap)
                {
                    const std::optional<Ref<Texture2D>> albedo = _material->GetTextureAt(MaterialTextureIndex::Albedo);
                    DENTY_CHECK_CONDITION(albedo.has_value());

                    ImGui::PushID("Albedo");
                    if (ImGui::ImageButton(_textureXMarkRef->GetResourceAsComPtr().Get(), CROSS_MARK_SIZE.ToImVec2()))
                    {
                        _material->RemoveTextureAt(MaterialTextureIndex::Albedo);
                    }
                    ImGui::PopID();

                    if (ImGui::ImageButton(albedo.value()->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Albedo;
                        _isOpenSelectTextureWindow = true;
                    }

                	ImGui::SameLine();

                    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.4f);
                    ImGui::SliderFloat("##AlbedoBaseRate", &_material->parameters.baseRate, 0.0f, 1.0f);
                }
                else
                {
                    ImGui::PushID("Albedo_Empty");
                    if (ImGui::ImageButton(_textureEmptyRef->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Albedo;
                        _isOpenSelectTextureWindow = true;
                    }
                    ImGui::PopID();

                    ImGui::SameLine();

                    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.4f);
                    ImGui::ColorEdit4("##AlbedoColorEdit", &_material->parameters.baseColor.color.x);
                }
            }

            ImGui::PushTextWrapPos(windowSize.x * 0.5f);
            ImGui::TextWrapped("Metallic");
            ImGui::PopTextWrapPos();

            // Metallic
            {
                const bool hasMetalnessMap = _material->HasTextureAt(MaterialTextureIndex::Metalness);
                if (hasMetalnessMap)
                {
                    const std::optional<Ref<Texture2D>> metalness = _material->GetTextureAt(MaterialTextureIndex::Metalness);
                    DENTY_CHECK_CONDITION(metalness.has_value());

                    ImGui::PushID("Metallic");
                    if (ImGui::ImageButton(_textureXMarkRef->GetResourceAsComPtr().Get(), CROSS_MARK_SIZE.ToImVec2()))
                    {
                        _material->RemoveTextureAt(MaterialTextureIndex::Metalness);
                    }
                    ImGui::PopID();

                    if (ImGui::ImageButton(metalness.value()->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Metalness;
                        _isOpenSelectTextureWindow = true;
                    }
                }
                else
                {
                    ImGui::PushID("Metalness_Empty");
                    if (ImGui::ImageButton(_textureEmptyRef->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Metalness;
                        _isOpenSelectTextureWindow = true;
                    }
                    ImGui::PopID();

                	ImGui::SameLine();

                	ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.4f);
                    ImGui::SliderFloat("##Metallic", &_material->parameters.metallic, 0.0f, 1.0f);
                }
            }

            ImGui::PushTextWrapPos(windowSize.x * 0.5f);
            ImGui::TextWrapped("Normal");
            ImGui::PopTextWrapPos();

            // Normal
            {
                const bool hasNormalMap = _material->HasTextureAt(MaterialTextureIndex::Normal);
                if (hasNormalMap)
                {
                    const std::optional<Ref<Texture2D>> normal = _material->GetTextureAt(MaterialTextureIndex::Normal);
                    DENTY_CHECK_CONDITION(normal.has_value());

                    ImGui::PushID("Normal");
                    if (ImGui::ImageButton(_textureXMarkRef->GetResourceAsComPtr().Get(), CROSS_MARK_SIZE.ToImVec2()))
                    {
                        _material->RemoveTextureAt(MaterialTextureIndex::Normal);
                    }
                    ImGui::PopID();

                    if (ImGui::ImageButton(normal.value()->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Normal;
                        _isOpenSelectTextureWindow = true;

                    }
                }
                else
                {
                    ImGui::PushID("Normal_Empty");
                    if (ImGui::ImageButton(_textureEmptyRef->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Normal;
                        _isOpenSelectTextureWindow = true;
                    }
                    ImGui::PopID();
                }
            }

            ImGui::PushTextWrapPos(windowSize.x * 0.5f);
            ImGui::TextWrapped("Roughness");
            ImGui::PopTextWrapPos();

            // Roughness
            {
                const bool hasRoughnessMap = _material->HasTextureAt(MaterialTextureIndex::Roughness);
                if (hasRoughnessMap)
                {
                    const std::optional<Ref<Texture2D>> roughness = _material->GetTextureAt(MaterialTextureIndex::Roughness);
                    DENTY_CHECK_CONDITION(roughness.has_value());

                    ImGui::PushID("Roughness");
                    if (ImGui::ImageButton(_textureXMarkRef->GetResourceAsComPtr().Get(), CROSS_MARK_SIZE.ToImVec2()))
                    {
                        _material->RemoveTextureAt(MaterialTextureIndex::Roughness);
                    }
                    ImGui::PopID();

                    if (ImGui::ImageButton(roughness.value()->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Roughness;
                        _isOpenSelectTextureWindow = true;
                    }
                }
                else
                {
                    ImGui::PushID("Roughness_Empty");
                    if (ImGui::ImageButton(_textureEmptyRef->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Roughness;
                        _isOpenSelectTextureWindow = true;
                    }
                    ImGui::PopID();

                    ImGui::SameLine();

                    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.4f);
                    ImGui::SliderFloat("##Roughness", &_material->parameters.roughness, 0.0f, 1.0f);
                }
            }

            ImGui::PushTextWrapPos(windowSize.x * 0.5f);
            ImGui::TextWrapped("Emissive");
            ImGui::PopTextWrapPos();

            // Emissive
            {
                const bool hasEmissiveMap = _material->HasTextureAt(MaterialTextureIndex::Emissive);
                if (hasEmissiveMap)
                {
                    const std::optional<Ref<Texture2D>> emissive = _material->GetTextureAt(MaterialTextureIndex::Emissive);
                    DENTY_CHECK_CONDITION(emissive.has_value());

                    ImGui::PushID("Emissive");
                    if (ImGui::ImageButton(_textureXMarkRef->GetResourceAsComPtr().Get(), CROSS_MARK_SIZE.ToImVec2()))
                    {
                        _material->RemoveTextureAt(MaterialTextureIndex::Emissive);
                    }
                    ImGui::PopID();

                    if (ImGui::ImageButton(emissive.value()->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Emissive;
                        _isOpenSelectTextureWindow = true;

                    }

                    ImGui::SameLine();

                    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.4f);
                    ImGui::SliderFloat("##EmissiveIntensity", &_material->parameters.emissiveIntensity, 1.0f, 8.0f);
                }
                else
                {
                    ImGui::PushID("Emissive_Empty");
                    if (ImGui::ImageButton(_textureEmptyRef->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::Emissive;
                        _isOpenSelectTextureWindow = true;
                    }
                    ImGui::PopID();

                    ImGui::SameLine();

                    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.4f);
                    ImGui::ColorEdit4("#EmissiveColor", &_material->parameters.emissiveColor.color.x);

                    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.4f);
                    ImGui::SliderFloat("##EmissiveIntensity", &_material->parameters.emissiveIntensity, 0.0f, 10.0f);
                }
            }

            ImGui::PushTextWrapPos(windowSize.x * 0.5f);
            ImGui::TextWrapped("Ambient Occlusion");
            ImGui::PopTextWrapPos();

            // Ambient occlusion
            {
                const bool hasAmbientOcclusionMap = _material->HasTextureAt(MaterialTextureIndex::AmbientOcclusion);
                if (hasAmbientOcclusionMap)
                {
                    const std::optional<Ref<Texture2D>> ambientOcclusion = _material->GetTextureAt(MaterialTextureIndex::AmbientOcclusion);
                    DENTY_CHECK_CONDITION(ambientOcclusion.has_value());

                    ImGui::PushID("Ambient occlusion");
                    if (ImGui::ImageButton(_textureXMarkRef->GetResourceAsComPtr().Get(), CROSS_MARK_SIZE.ToImVec2()))
                    {
                        _material->RemoveTextureAt(MaterialTextureIndex::AmbientOcclusion);
                    }
                    ImGui::PopID();

                    if (ImGui::ImageButton(ambientOcclusion.value()->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::AmbientOcclusion;
                        _isOpenSelectTextureWindow = true;
                    }
                }
                else
                {
                    ImGui::PushID("AmbientOcclusion_Empty");
                    if (ImGui::ImageButton(_textureEmptyRef->GetResourceAsComPtr().Get(), THUMBNAIL_SIZE.ToImVec2()))
                    {
                        _selectTexture = MaterialEditorSelectTexture::AmbientOcclusion;
                        _isOpenSelectTextureWindow = true;
                    }
                    ImGui::PopID();
                }
            }
        }
        ImGui::End();

        if (_selectTexture != MaterialEditorSelectTexture::None)
        {
            const std::optional<Ref<Texture2D>> texture2d = EditorHelper::SelectTexture();
            if (texture2d.has_value())
            {
                if (_material->HasTextureAt(static_cast<MaterialTextureIndex>(_selectTexture)))
                {
                    _material->ReplaceTextureAt(texture2d.value(), static_cast<MaterialTextureIndex>(_selectTexture));
                }
                else
                {
                    _material->SetTextureAt(texture2d.value(), static_cast<MaterialTextureIndex>(_selectTexture));
                }

                _selectTexture = MaterialEditorSelectTexture::None;
                _isOpenSelectTextureWindow = false;
            }
        }

        // If closed.
        if (not _open)
        {
            const bool isChanged = !_material->IsEqual(_oldMaterial);
            if (isChanged)
            {
                _material->Save(true);
            }

            // Notify event.
            EventOnMaterialEditorClosed eventOnMaterialEditorClosed(_material->GetFilePathToSerialized(), isChanged);
            NOTIFY_EVENT(eventOnMaterialEditorClosed);
        }
    }

    void MaterialEditor::OnOpened(EventOnMaterialEditorOpened* e)
    {
        _open = true;
        _oldMaterial = std::make_shared<Material>();
        e->GetMaterial()->Clone(_oldMaterial);

        _material = e->GetMaterial();

        _renderingMode = std::string(NAMEOF_ENUM(_material->parameters.renderingMode));

        ImGui::SetNextWindowSize(ImVec2(IMGUI_DEFAULT_WINDOW_SIZE.x, IMGUI_DEFAULT_WINDOW_SIZE.y));
    }

    void MaterialEditor::OnClosed([[maybe_unused]] EventOnMaterialEditorClosed* e)
    {
        // Reset member variables.
        {
            _open = false;
            _material = nullptr;
            _oldMaterial = nullptr;
            _renderingMode = std::string();
        }
    }

	void MaterialEditor::TextureAssetDragAndDropTargetCallback(void* data, DragAndDropDataType dataType, const Ref<void>& userData)
    {
        DENTY_CHECK_CONDITION(dataType == DragAndDropDataType::Texture)

        const auto id = *static_cast<TextureID*>(data);

        const TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

        const std::optional<Ref<Texture2D>> texture2d = textureLibrary->Find<Texture2D>(id);
        if (texture2d.has_value())
        {
        }
        else
        {
        }
    }
}
