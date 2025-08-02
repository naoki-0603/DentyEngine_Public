#include "stdafx.h"

#include "ShadowMap.h"

#include "Graphics/Buffer/ColorBuffer.h"
#include "Graphics/Buffer/FrameBuffer.h"

#include "Graphics/Renderer/ShadowCamera.h"

#include "Component/Graphics/Renderer/DirectionalLight.h"
#include "Graphics/Renderer/Texture2D.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
    Scope<ShadowMap> ShadowMap::_instance = nullptr;

    ShadowMap::ShadowMap() :
        _texture2d(), _shadowCamera(), _frameBuffer(), _type(ShadowMapType::Soft), _size(2048, 2048)
    {
    }

    void ShadowMap::Create()
    {
        // Create shadow camera.
        {
            _shadowCamera = std::make_unique<ShadowCamera>();
            _shadowCamera->Create();
        }

        // Create frame buffer.
        {
            ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

            _frameBuffer = std::make_unique<FrameBuffer>();
            _frameBuffer->Create(
                device,
                _size,
                BlendState::Type::NoBlend,
                DXGI_FORMAT_R16G16B16A16_FLOAT,
                DXGI_FORMAT_R32_TYPELESS,
                FrameBuffer::Usage::UseBoth, true, false
            );
        }

        // Create texture2d for use expanded image gui.
        {
            _texture2d = std::make_unique<Texture2D>();
        }
    }

    void ShadowMap::OnUpdate()
    {
        _shadowCamera->OnUpdate();
    }

    void ShadowMap::OnEvent([[maybe_unused]] Event* e)
    {
    }

    void ShadowMap::OnGui()
    {
        if (ImGui::TreeNodeEx("ShadowCamera##ShadowMap", ImGuiTreeNodeFlags_DefaultOpen))
        {
            _shadowCamera->OnGui();

            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Buffer##ShadowMap", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Text("Size: %f, %f", _size.x, _size.y);
            ImGui::Text("Type: %s", std::string(NAMEOF_ENUM(_type)).c_str());

            // Set shader resource view.
            _texture2d->SetShaderResourceView(_frameBuffer->GetColorBuffer()->GetResourceAsComPtr());
            _texture2d->ExpandedImageGui(Vector2(256, 256));

            ImGui::TreePop();
        }

    }

    void ShadowMap::Begin([[maybe_unused]] ID3D11DeviceContext* context)
    {
        /*_frameBuffer->Clear(context, Color::CLEAR);
        _frameBuffer->Activate(context);*/
    }

    void ShadowMap::End([[maybe_unused]] ID3D11DeviceContext* context)
    {
        //_frameBuffer->DeActivate(context);
    }

    void ShadowMap::BindCamera([[maybe_unused]] ID3D11DeviceContext* context)
    {
        //_shadowCamera->Bind(context);
    }

    void ShadowMap::SetDirectionalLight([[maybe_unused]] Ref<DirectionalLight> directionalLight)
    {
        //_shadowCamera->SetDirectionalLight(directionalLight);
    }

    ID3D11ShaderResourceView* const* ShadowMap::GetShadowMap() const
    {
        return _frameBuffer->GetColorBuffer()->GetResource();
    }

	Scope<ShadowMap>& ShadowMap::GetInstance()
	{
		if (!_instance)
			_instance = std::make_unique<ShadowMap>();

		return _instance;
	}
}
