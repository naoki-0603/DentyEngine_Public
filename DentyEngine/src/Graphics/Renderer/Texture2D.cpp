#include "stdafx.h"

#include "Texture2D.h"

#include "Shader.h"

#include "Editor/Parts/EditorContentBrowser.h"

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Buffer/FrameBuffer.h"
#include "Graphics/PostEffect/FullScreenQuad.h"
#include "Graphics/States/SamplerState.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Resource/Library/ShaderLibrary.h"
#include "System/Resource/Library/TextureLibrary.h"
#include "System/Resource/Library/LibraryManager.h"

#include "System/Serializer/TextureSerializer.h"
#include "System/Serializer/SerializeExtensions.h"

#include "Utilities/StringUtilities.h"

namespace DentyEngine
{
    Texture2D::Texture2D() :
        Texture(), _callbackInfo(), _downSamplingTexture(), _fullScreenQuad(), _clipPosition(), _clipSize(), _color(),
		_colorFiltering(RED | GREEN | BLUE | ALPHA)
    {
        _type = Type::Texture2D;
    }

    Texture2D& Texture2D::operator=(const Texture2D& source)
    {
        Texture::operator=(source);

        _clipPosition = source._clipPosition;
        _clipSize = source._clipSize;
        _color = source._color;

        return *this;
    }

    void Texture2D::Load(ID3D11Device* device, const std::filesystem::path& filePath, bool serialize)
    {
        _name = filePath.stem().string();

        ComRef<ID3D11Resource> textureResource = {};
        HRESULT hr = S_OK;

    	// Create texture
        {
            const std::wstring path = StringUtils::StringToWString(filePath.string());

            hr = DirectX::CreateWICTextureFromFile(
                device, path.c_str(), 
                textureResource.GetAddressOf(), _shaderResourceView.ReleaseAndGetAddressOf()
            );

            if (FAILED(hr))
            {
                DENTY_SET_ERR_COLOR

                DENTY_ERR_LOG << "Failed to load " << filePath << " texture!";

                return;
            }
        }

        // Extract data
        {
            D3D11_TEXTURE2D_DESC texture2dDesc = {};
            ComRef<ID3D11Texture2D> texture2d;

            hr = textureResource->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));

            texture2d->GetDesc(&texture2dDesc);

            this->_size = Vector2(static_cast<float>(texture2dDesc.Width), static_cast<float>(texture2dDesc.Height));
            this->_format = texture2dDesc.Format;
        }

        // Convert to .DentyTexture format.
        {
            _filePath = filePath;
            _filePathToSerialized = FilePath(filePath).ReplaceExtension(TEXTURE_FILE_EXTENSION);
        }

        // Generate down sampling texture
        {
            GenerateDownSamplingTexture();
        }

        if (serialize)
        {
            // Save
            {
                Save();
            }

            // Add to library.
            {
                TextureLibrary* textureLibrary = LibraryManager::GetInstance().GetTextureLibrary();

                textureLibrary->Add(shared_from_this());
            }
        }
    }

    void Texture2D::Load(ID3D11Device* device, const std::filesystem::path& filePath, const Vector2& clipPosition,
	    const Vector2& clipSize, const Color& color, bool serialize)
    {
        _clipPosition = clipPosition;
        _clipSize = clipSize;
        _color = color;

        Load(device, filePath, serialize);
    }

    void Texture2D::CreateDummyTexture(ID3D11Device* device, const UINT dimension)
	{
        HRESULT hr = S_OK;

        D3D11_TEXTURE2D_DESC texture2dDesc = {};

        // Set dummy texture parameter
        {
            texture2dDesc.Width = dimension;
            texture2dDesc.Height = dimension;
            texture2dDesc.MipLevels = 1;
            texture2dDesc.ArraySize = 1;
            texture2dDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            texture2dDesc.SampleDesc.Count = 1;
            texture2dDesc.SampleDesc.Quality = 0;
            texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
            texture2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        }

        // Allocate an area for a texel
        UINT texels = dimension * dimension;
        auto systemMemory = std::make_unique<DWORD[]>(texels);
        {
            for (UINT i = 0; i < texels; ++i)
            {
                systemMemory[i] = 0xFFFFFFFF;
            }
        }

        // Register data in the buffer allocated by system memory
        D3D11_SUBRESOURCE_DATA subresourceData = {};
        {
            subresourceData.pSysMem = systemMemory.get();

            // Bytes per line.
            subresourceData.SysMemPitch = sizeof(DWORD) * dimension;
        }

        // Create buffer
        ComRef<ID3D11Texture2D> texture2d;
        {
            hr = device->CreateTexture2D(&texture2dDesc, &subresourceData, &texture2d);
            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        // Create shader resource view
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
            shaderResourceViewDesc.Format = texture2dDesc.Format;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            hr = device->CreateShaderResourceView(
                texture2d.Get(),
                &shaderResourceViewDesc,
                _shaderResourceView.GetAddressOf()
            );
            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        _name = "DummyTexture";
        _isDummyTexture = true;
	}

    void Texture2D::Save(bool isUpdate) const
    {
        // If failed.
	    if (!TextureSerializer::SerializeAsJson(_filePathToSerialized.Get(), shared_from_this()))
	    {
            DENTY_SET_ERR_COLOR

            if (isUpdate)
            {
                DENTY_ERR_LOG << "Failed to update " << _filePathToSerialized << " texture!";
            }
            else
            {
                DENTY_ERR_LOG << "Failed to save " << _filePathToSerialized << " texture!";
            }

            return;
	    }

        DENTY_SET_INFO_COLOR

        if (isUpdate)
        {
            DENTY_INFO_LOG << "Succeed to update " << _filePathToSerialized << " texture!";
        }
        else
        {
            DENTY_INFO_LOG << "Succeed to save " << _filePathToSerialized << " texture!";
        }
    }

    void Texture2D::Load(const std::filesystem::path& filePath)
    {
        auto texture2d = std::make_shared<Texture2D>();

        // If failed
        if (!TextureSerializer::DeserializeFromJson(filePath, texture2d))
        {
            DENTY_SET_ERR_COLOR

            DENTY_ERR_LOG << "Failed to load " << filePath << " texture!";

            return;
        }

        texture2d->Clone(shared_from_this());

        DENTY_SET_INFO_COLOR
        DENTY_INFO_LOG << "Succeed to load " << filePath << " texture!";
    }

    void Texture2D::Rename(std::string_view newName, bool deleteOldFile)
    {
	    Texture::Rename(newName, deleteOldFile);
    }

    void Texture2D::Clone(const Ref<Asset>& asset)
    {
        Texture::Clone(asset);

        auto&& target = std::static_pointer_cast<Texture2D>(asset);
        target->_clipPosition = _clipPosition;
        target->_clipSize = _clipSize;
        target->_color = _color;
    }

    void Texture2D::OnEvent(Event* e)
    {
	    Texture::OnEvent(e);
    }

    void Texture2D::OnDestroy()
    {
	    Texture::OnDestroy();
    }

    void Texture2D::OnSerialized() const
    {
	    Texture::OnSerialized();
    }

    void Texture2D::OnDeserialized()
    {
	    Texture::OnDeserialized();

        if (not _filePath.IsEmpty())
        {
            if (not _shaderResourceView)
            {
                ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

                Load(device, _filePath.Get());
            }
        }
    }

    // https://qiita.com/atsds/items/b94eab1e3042c34c829e
    void Texture2D::ExpandedImageGui(Vector2 size)
    {
        ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

        const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();
        const auto shader = shaderLibrary->Find(ShaderLibrary::EXPANDED_IMAGE_SHADER_PATH).value();

        ColorFilterGui();

        {
            _callbackInfo.context = context;
            _callbackInfo.samplerState = SamplerState::GetBuildInSamplerStatesAt(SamplerState::Type::PointWrap);
            _callbackInfo.shader = shader;

            ExpandedImageConstants constants = {};
            if (_colorFiltering & RED)
            {
                constants.redElementEnabled = 1;
            }

            if (_colorFiltering & GREEN)
            {
                constants.greenElementEnabled = 1;
            }

            if (_colorFiltering & BLUE)
            {
                constants.blueElementEnabled = 1;
            }

            if (_colorFiltering & ALPHA)
            {
                constants.alphaElementEnabled = 1;
            }

            _callbackInfo.constants = constants;
        }

        // Change pixel shader.
        ImGui::GetWindowDrawList()->AddCallback(
            []([[maybe_unused]] const ImDrawList* parentList, const ImDrawCmd* cmd)
            {
                const auto expandedImageCallback = static_cast<ExpandedImageCallbackInfo*>(cmd->UserCallbackData);
                expandedImageCallback->shader->constantBuffer->UpdateSubresource(expandedImageCallback->context, &expandedImageCallback->constants);
                expandedImageCallback->shader->constantBuffer->BindAll(expandedImageCallback->context, 1u);

                expandedImageCallback->shader->BindVertexShader(expandedImageCallback->context);
                expandedImageCallback->shader->BindPixelShader(expandedImageCallback->context);

                expandedImageCallback->context->PSSetSamplers(0, 1, &expandedImageCallback->samplerState);
            }, &_callbackInfo
        );

        // Draw
        ImGui::Image(_shaderResourceView.Get(), size.ToImVec2());

        // Set original state.
        ImGui::GetWindowDrawList()->AddCallback(
            ImDrawCallback_ResetRenderState, nullptr
        );
    }

    void Texture2D::SetShaderResourceView(ComRef<ID3D11ShaderResourceView> shaderResourceView)
    {
        if (_shaderResourceView)
            return;

        if (!shaderResourceView)
            return;

        shaderResourceView.CopyTo(_shaderResourceView.GetAddressOf());
    }

    void Texture2D::GenerateDownSamplingTexture()
    {
        ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();
        ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

        const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

        _downSamplingTexture = Utils::CreateRef<FrameBuffer>();
        _downSamplingTexture->Create(
            device,
            EditorContentBrowser::DOWN_SAMPLING_THUMBNAIL_SIZE,
            BlendState::Type::Alpha,
            DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, 
            FrameBuffer::Usage::UseRenderTargetView, true, false
        );

        _fullScreenQuad = Utils::CreateRef<FullScreenQuad>();

        _downSamplingTexture->Clear(context, Color::GRAY);
        _downSamplingTexture->Activate(context);

        FullScreenQuad::SrvInfo srvInfo = {};
        srvInfo.shaderResourceViews = _shaderResourceView.GetAddressOf();
        srvInfo.startSlot = 0u;
        srvInfo.viewsNum = 1u;

        FullScreenQuad::ConstantInfo constantInfo = {};
        constantInfo.startSlot = 0u;
        constantInfo.constants = nullptr;

        /*const Ref<Shader> fullScreenQuadShader = shaderLibrary->Find(ShaderLibrary::FULL_SCREEN_QUAD_PATH).value();

        _fullScreenQuad->Blit(fullScreenQuadShader, srvInfo, constantInfo);*/

        _downSamplingTexture->DeActivate(context);
    }

    void Texture2D::ColorFilterGui()
    {
        // Filtering
        ImGui::Text("Filtering");
        ImGui::SameLine();

        // R
        {
            bool pushStyle = false;
            if (_colorFiltering & RED)
            {
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));

                pushStyle = true;
            }

            if (ImGui::ColorButton("Red##TextureEditor", ImVec4(1, 0, 0, 1)))
            {
                if (_colorFiltering & RED)
                {
                    _colorFiltering = _colorFiltering & ~RED;
                }
                else
                {
                    _colorFiltering |= RED;
                }
            }

            if (pushStyle)
            {
                ImGui::PopStyleColor();
            }
        }

        ImGui::SameLine();

        // G
        {
            bool pushStyle = false;
            if (_colorFiltering & GREEN)
            {
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));

                pushStyle = true;
            }

            if (ImGui::ColorButton("Green##TextureEditor", ImVec4(0, 1, 0, 1)))
            {
                if (_colorFiltering & GREEN)
                {
                    _colorFiltering = _colorFiltering & ~GREEN;
                }
                else
                {
                    _colorFiltering |= GREEN;
                }
            }

            if (pushStyle)
            {
                ImGui::PopStyleColor();
            }
        }


        ImGui::SameLine();

        // B
        {
            bool pushStyle = false;
            if (_colorFiltering & BLUE)
            {
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));

                pushStyle = true;
            }

            if (ImGui::ColorButton("Blue##TextureEditor", ImVec4(0, 0, 1, 1)))
            {
                if (_colorFiltering & BLUE)
                {
                    _colorFiltering = _colorFiltering & ~BLUE;
                }
                else
                {
                    _colorFiltering |= BLUE;
                }
            }

            if (pushStyle)
            {
                ImGui::PopStyleColor();
            }
        }

        ImGui::SameLine();

        // A
        {
            bool pushStyle = false;
            if (_colorFiltering & ALPHA)
            {
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));

                pushStyle = true;
            }

            if (ImGui::ColorButton("Alpha##TextureEditor", ImVec4(1, 1, 1, 1)))
            {
                if (_colorFiltering & ALPHA)
                {
                    _colorFiltering = _colorFiltering & ~ALPHA;
                }
                else
                {
                    _colorFiltering |= ALPHA;
                }
            }

            if (pushStyle)
            {
                ImGui::PopStyleColor();
            }
        }

        {
            std::string selectedColorFiltering = {};

            if (_colorFiltering & RED)
            {
                selectedColorFiltering += "R";
            }

            if (_colorFiltering & GREEN)
            {
                selectedColorFiltering += "G";
            }

            if (_colorFiltering & BLUE)
            {
                selectedColorFiltering += "B";
            }

            if (_colorFiltering & ALPHA)
            {
                selectedColorFiltering += "A";
            }

            if (_colorFiltering == 0)
            {
                selectedColorFiltering = "None";
            }

            ImGui::SameLine();
            ImGui::Text("Selected color filtering: %s", selectedColorFiltering.c_str());
        }
    }
};