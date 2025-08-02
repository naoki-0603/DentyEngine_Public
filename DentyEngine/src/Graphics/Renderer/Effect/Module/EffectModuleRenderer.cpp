#include "stdafx.h"

#include "EffectModuleRenderer.h"

#include "Graphics/Renderer/Texture2D.h"

namespace DentyEngine
{
    EffectModuleRendererCPUContext::EffectModuleRendererCPUContext() :
        dxContext(), vertexBuffer()
    {
    }

    EffectModuleRendererConstants::EffectModuleRendererConstants() :
		world(),
        albedoColor(), specularColor(), ambientColor(), emissionColor(),
        metallic(), roughness(), kd(), ks(), ka(), padding()
    {
    }

    EffectModuleRenderer::EffectModuleRenderer() :
        EffectModule(), _material(), _constants(), _renderingOrder()
    {
        _name = "EffectModuleRenderer";
        _enabled = true;
    }

    EffectModuleRenderer::EffectModuleRenderer(const EffectModuleRenderer& source) :
        EffectModule(source), _material(source._material), _renderingOrder(source._renderingOrder)
    {
    }

    void EffectModuleRenderer::Initialize()
    {
    }

    void EffectModuleRenderer::OnGui()
    {

    }

    void EffectModuleRenderer::OnUpdateCPU(const Matrix& world)
    {
        _constants.world = world;

        /*_constants.albedoColor = _material->albedoColor;
        _constants.specularColor = _material->specularColor;
        _constants.ambientColor = _material->ambientColor;
        _constants.emissionColor = _material->emissionColor;
        _constants.roughness = _material->roughness;
        _constants.metallic = _material->metallic;
        _constants.kd = _material->kd;
        _constants.ks = _material->ks;
        _constants.ka = _material->ka;
        _constants.textureUsed = _material->textureUsed;*/
    }

    void EffectModuleRenderer::OnRenderCPU(const EffectModuleRendererCPUContext& context)
    {
        //context.dxContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        //context.vertexBuffer->Bind();

        ///*_material->shader->BindVertexShader(context.dxContext);
        //_material->shader->BindPixelShader(context.dxContext);

        //_material->shader->constantBuffer->UpdateSubresource(context.dxContext, &_constants);
        //_material->shader->constantBuffer->BindAll(context.dxContext, 0u);

        //auto&& albedoOptional = _material->FindTextureByAlias("Albedo");*/
        //if (albedoOptional.has_value())
        //{
        //    context.dxContext->PSSetShaderResources(0u, 1u, albedoOptional.value()->GetResource());
        //}

        //context.dxContext->DrawAuto();
    }

    EffectModuleType EffectModuleRenderer::GetType() const
    {
        return EffectModuleType::Renderer;
    }

    EffectRenderingOrder EffectModuleRenderer::GetRenderingOrder() const
    {
        return _renderingOrder;
    }
}
