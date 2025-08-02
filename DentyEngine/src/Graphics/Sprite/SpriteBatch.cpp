#include "stdafx.h"

#include "SpriteBatch.h"

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Buffer/DirectX/DXIndexBuffer.h"
#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"
#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/Texture2D.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
    SpriteBatchResource::SpriteBatchResource() :
        texture(), vertices(), indices(), vertexBuffer(), indexBuffer()
    {
    }

    void SpriteBatchResource::Create(Ref<Texture2D> texture2D, SpriteSortMode mode, bool isWorld)
    {
        texture = texture2D;

        if (mode == SpriteSortMode::Immediate)
        {
            vertices.resize(4u * SpriteBatch::MAX_IMMEDIATE_SPRITE_COUNT);
            indices.resize(6u * SpriteBatch::MAX_IMMEDIATE_SPRITE_COUNT);
        }
        else
        {
            vertices.resize(4u * SpriteBatch::MAX_SPRITE_COUNT);
            indices.resize(6u * SpriteBatch::MAX_SPRITE_COUNT);
        }
    }

    void SpriteBatchResource::CreateBuffers()
    {
        DENTY_ASSERT(!vertices.empty() && !indices.empty(),
            "Vertices or indices is empty! (in SpriteBatchResource::CreateBuffers");

        // Vertex buffer.
        {
            
        }

        // Index buffer.
        {
            
        }
    }

    SpriteBatchConstants::SpriteBatchConstants() :
        id(), padding(), world()
    {
    }

    SpriteBatch::SpriteBatch() :
        Object(), _resource(), _constants(), _indexBufferOffset(),
        _isWorld(), _isInImmediateMode(), _isBeginCalled()
    {
        _name = "SpriteBatch";
    }

    void SpriteBatch::Create(Ref<Texture2D> texture2D, SpriteSortMode mode, bool isWorld)
    {
        _resource.Create(texture2D, mode, isWorld);

        _sortMode = mode;
        _isWorld = isWorld;
    }

    void SpriteBatch::Begin(BlendState::Type blendStateType, DepthStencilState::Type depthStencilType,
                            RasterizerState::Type rasterizerStateType, SamplerState::Type samplerStateType)
    {
        _blendStateType = blendStateType;
        _depthStencilStateType = depthStencilType;
        _rasterizerStateType = rasterizerStateType;
        _samplerStateType = samplerStateType;

        if (_sortMode == SpriteSortMode::Immediate)
        {
            if (_isInImmediateMode)
                DENTY_ASSERT(false, "One sprite only! (in SpriteBatch::Begin)");

            PrepareForRendering();

            _isInImmediateMode = true;
        }

        _isBeginCalled = true;
    }

    void SpriteBatch::RenderOnScreen(const Vector2& position, float rotation, float scale, const Color& color)
    {
        if (_isWorld)
            DENTY_ASSERT(false, "Can not render when is world enabled! (in SpriteBatch::RenderOnScreen)");

        if (_sortMode == SpriteSortMode::Immediate)
        {
            RenderBatchOnScreen(position, rotation, scale, color, 1u);
        }
    }

    void SpriteBatch::RenderOnWorld(const Vector3& position, const Color& color)
    {
        if (!_isWorld)
            DENTY_ASSERT(false, "Set enable isWorld flag! (in SpriteBatch::RenderOnWorld)");

        if (_sortMode == SpriteSortMode::Immediate)
        {
            RenderBatchOnWorld(position, color, 1u);
        }
    }

    void SpriteBatch::End()
    {
        if (!_isBeginCalled)
            DENTY_ASSERT(false, "Begin must be call before end called! (in SpriteBatch::End)");

        // Already drawn.
        if (_isInImmediateMode)
        {
            _isInImmediateMode = false;
        }
        // Draw sprites.
        else
        {

        }

        _isBeginCalled = false;
    }

    void SpriteBatch::PrepareForRendering()
    {
        ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

        // Bind states.
        {
            constexpr FLOAT blendFactor[4] = { 1,1,1,1 };
            ID3D11BlendState* blendState = BlendState::GetBuildInBlendStatesAt(_blendStateType);
			ID3D11DepthStencilState* depthStencilState = DepthStencilState::GetBuildInDepthStencilStatesAt(_depthStencilStateType);
			ID3D11RasterizerState* rasterizerState = RasterizerState::GetBuildInRasterizerStatesAt(_rasterizerStateType);
			ID3D11SamplerState* samplerState = SamplerState::GetBuildInSamplerStatesAt(_samplerStateType);

			context->OMSetBlendState(blendState, blendFactor, 0xFFFFFFFF);
			context->OMSetDepthStencilState(depthStencilState, 0);
			context->PSSetSamplers(0, 1, &samplerState);
			context->RSSetState(rasterizerState);
        }

        // Bind buffer and shaders.
        {
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            // Buffers.
            {
                _resource.vertexBuffer->Bind();
                _resource.indexBuffer->Bind();
            }

            // Shaders.
            {
                const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

                if (_isWorld)
                {
                    const std::optional<Ref<Shader>> shader = shaderLibrary->Find(ShaderLibrary::SPRITE_3D_SHADER_PATH);
                    if (!shader.has_value())
                    {
                        DENTY_ASSERT(false, "Shader not found! (in SpriteBatch::PrepareForRendering)");
                    }

                    shader.value()->constantBuffer->UpdateSubresource(context, &_constants);
                    shader.value()->constantBuffer->BindAll(context, 0u);

                    shader.value()->BindVertexShader(context);
                    shader.value()->BindPixelShader(context);
                }
                else
                {
                    const std::optional<Ref<Shader>> shader = shaderLibrary->Find(ShaderLibrary::SPRITE_SHADER_PATH);
                    if (!shader.has_value())
                    {
                        DENTY_ASSERT(false, "Shader not found! (in SpriteBatch::PrepareForRendering)");
                    }

                    shader.value()->constantBuffer->UpdateSubresource(context, &_constants);
                    shader.value()->constantBuffer->BindAll(context, 0u);

                    shader.value()->BindVertexShader(context);
                    shader.value()->BindPixelShader(context);
                }
            }
        }
    }

    void SpriteBatch::RenderBatchOnWorld(const Vector3& position, const Color& color, uint32_t spriteCount)
    {
        ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

        context->PSSetShaderResources(0, 1, _resource.texture->GetResource());

        const Vector2 textureSize = _resource.texture->GetSize();
    }

    void SpriteBatch::RenderBatchOnScreen(const Vector2& position, float rotation, float scale, const Color& color, uint32_t spriteCount)
    {
        ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

        context->PSSetShaderResources(0, 1, _resource.texture->GetResource());

        const Vector2 textureSize = _resource.texture->GetSize();

        // Should be immediate mode.
        if (spriteCount == 1u)
        {
            // Generate vertices.
            RenderSpriteOnScreen(position, textureSize, rotation, scale, color);
        }
    }

    void SpriteBatch::RenderSpriteOnWorld(const Vector3& position, const Vector2& textureSize, const Color& color)
    {

    }

    void SpriteBatch::RenderSpriteOnScreen(const Vector2& position, const Vector2& textureSize, float rotation, float scale, const Color& color)
    {
        
    }
}
