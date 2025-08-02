#include "stdafx.h"

#include "Sprite.h"

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Buffer/DirectX/DXVertexBuffer.h"
#include "Graphics/Renderer/Shader.h"
#include "Graphics/Renderer/Texture2D.h"

#include "Math/AffineTransform.h"
#include "Math/CoordinateTransformation.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

namespace DentyEngine
{
    namespace Utils
    {
        Vector2 CalculatePivotBasedPosition(const Vector2& position, const Vector2& textureSize, SpritePivot pivot)
        {
            switch (pivot)
            {
            case SpritePivot::TopLeft: return { position };
            case SpritePivot::TopRight: return { position.x + textureSize.x, position.y };
            case SpritePivot::BottomLeft: return { position.x, position.y + textureSize.y };
            case SpritePivot::BottomRight: return { position + textureSize };
            case SpritePivot::Center: return { position.x + textureSize.x * 0.5f, position.y + textureSize.y * 0.5f };
            }
            DENTY_ASSERT(false, "Unknown sprite pivot. (in Utils::CalculatePivotBasedPosition)");

            return {};
        }
    }


    SpriteResource::SpriteResource() :
        texture(), vertices(), vertexBuffer()
    {
    }

    void SpriteResource::Create(Ref<Texture2D> paramTexture, SpritePivot paramPivot)
    {
        this->texture = paramTexture;
        this->pivot = paramPivot;

        const Vector2& size = this->texture->GetSize();

        // Generate vertices and buffer.
        {
            SpriteVertex& topLeftVertex = vertices.emplace_back();
            topLeftVertex.position = { 0, 0, 1, 1 };
            topLeftVertex.color = Color::WHITE;
            topLeftVertex.texcoord = { 0, 0 };

            SpriteVertex& topRightVertex = vertices.emplace_back();
            topRightVertex.position = { size.x, 0, 1, 1};
            topRightVertex.color = Color::WHITE;
            topRightVertex.texcoord = { 1, 0 };

            SpriteVertex& bottomLeftVertex = vertices.emplace_back();
            bottomLeftVertex.position = { 0, size.y, 1, 1 };
            bottomLeftVertex.color = Color::WHITE;
            bottomLeftVertex.texcoord = { 0, 1 };

            SpriteVertex& bottomRightVertex = vertices.emplace_back();
            bottomRightVertex.position = { size.x, size.y, 1, 1 };
            bottomRightVertex.color = Color::WHITE;
            bottomRightVertex.texcoord = { 1, 1 };

            CreateBuffer();
        }
    }

    void SpriteResource::CreateBuffer()
    {
        DXVertexBuffer::VertexBufferInfo info = {};
        info.usage = DXVertexBuffer::Usage::Dynamic;
        info.accessFlag = DXVertexBuffer::CpuAccessFlag::AccessWrite;
        info.vertexType = DXVertexBuffer::VertexBufferInfo::VertexType::SpriteVertex;
        info.mapType = DXVertexBuffer::MapType::WriteDiscard;
        info.stride = sizeof(SpriteVertex);
        info.offset = 0u;
        info.count = static_cast<uint32_t>(vertices.size());
        info.size = info.stride * static_cast<uint32_t>(vertices.size());
        info.data = std::make_shared<SpriteVertex[]>(info.count);
        info.AssignData(vertices.data());

    	vertexBuffer = std::make_shared<DXVertexBuffer>();
        vertexBuffer->Create(info);
    }

    SpriteConstants::SpriteConstants() :
        id(), padding()
    {
    }

    Sprite::Sprite() :
        Object(), _resource(), _constants()
    {
        _name = "Sprite";
    }

    void Sprite::Create(Ref<Texture2D> texture, SpritePivot pivot)
    {
        _resource.Create(texture, pivot);

        _viewPortSize = {};
    }

    void Sprite::Update(EntityID id, const Vector2I& viewPortSize)
    {
        _constants.id = id;

        _viewPortSize = viewPortSize;
    }

    void Sprite::Render(const Vector2& position, const Color& color)
    {
        // If viewport size zero, doesn't render.
        if (_viewPortSize.Length() <= 0.0f)
            return;

        const Vector2& textureSize = _resource.texture->GetSize();

        float topLeftX, topLeftY;
        float topRightX, topRightY;
        float bottomLeftX, bottomLeftY;
        float bottomRightX, bottomRightY;

        // Convert to screen space to ndc coordinate.
        {
            topLeftX = position.x, topLeftY = position.y;
            topRightX = position.x + textureSize.x, topRightY = position.y;
            bottomLeftX = position.x, bottomLeftY = position.y + textureSize.y;
            bottomRightX = position.x + textureSize.x, bottomRightY = position.y + textureSize.y;

            CoordinateTransformation::ScreenToNdc(
                topLeftX, topLeftY, // TopLeftX, Y
                topRightX, topRightY, // TopRightX, Y
                bottomLeftX, bottomLeftY, // BottomRightX, Y,
                bottomRightX, bottomRightY, // BottomRightX, Y
                static_cast<float>(_viewPortSize.x), static_cast<float>(_viewPortSize.y)
            );
        }

        // Map/UnMap
        {
            void* data = _resource.vertexBuffer->Map();
            {
                if (data)
                {
                    const auto vertices = static_cast<SpriteVertex*>(data);

                    // Top left.
                    vertices[0].position = { topLeftX, topLeftY, 1.0f, 1.0f };
                    vertices[0].color = color;
                    vertices[0].texcoord = { 0, 0 };

                    // Top right.
                    vertices[1].position = { topRightX, topRightY, 1.0f, 1.0f };
                    vertices[1].color = color;
                    vertices[1].texcoord = { 1, 0 };

                    // Bottom Left
                    vertices[2].position = { bottomLeftX, bottomLeftY, 1.0f, 1.0f };
                    vertices[2].color = color;
                    vertices[2].texcoord = { 0, 1 };

                    // Bottom right
                    vertices[3].position = { bottomRightX, bottomRightY, 1.0f, 1.0f };
                    vertices[3].color = color;
                    vertices[3].texcoord = { 1, 1 };
                }
            }
            _resource.vertexBuffer->UnMap();
        }

        // Rendering pipeline
        {
            ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
            context->PSSetShaderResources(0, 1, _resource.texture->GetResource());

            // Bind buffer.
            {
                _resource.vertexBuffer->Bind();
            }

            // Bind shaders.
            {
                const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

                const std::optional<Ref<Shader>> shader = shaderLibrary->Find(ShaderLibrary::SPRITE_SHADER_PATH);
                if (!shader.has_value())
                {
                    DENTY_ASSERT(false, "Shader not found! (in Sprite::Render)");
                }

                shader.value()->constantBuffer->UpdateSubresource(context, &_constants);
                shader.value()->constantBuffer->BindAll(context, 0u);

                shader.value()->BindVertexShader(context);
                shader.value()->BindPixelShader(context);
            }

            context->Draw(4u, 0u);
        }
    }

    void Sprite::Render(const Vector2& position, float scale, float rotation, const Color& color)
    {
        // If viewport size zero, doesn't render.
        if (_viewPortSize.Length() <= 0.0f)
            return;

    	Vector2 textureSize = _resource.texture->GetSize();

        Vector2 pivotBasedPosition = {};

        // Calculate pivot based position.
        {
            pivotBasedPosition = Utils::CalculatePivotBasedPosition(position, textureSize, _resource.pivot);
        }

        Vector2 finalPosition = { };
        // Calculate rotation and scaling.
        {
            finalPosition += AffineTransform::Scale(
                position.x, position.y,
                pivotBasedPosition.x, pivotBasedPosition.y,
                scale, scale
            );

            finalPosition += AffineTransform::Rotate(
                position.x, position.y,
                pivotBasedPosition.x, pivotBasedPosition.y,
                std::cosf(rotation), std::sinf(rotation)
            );
        }

        float topLeftX, topLeftY;
        float topRightX, topRightY;
        float bottomLeftX, bottomLeftY;
        float bottomRightX, bottomRightY;

        // Convert to screen space to ndc coordinate.
        {
            topLeftX = finalPosition.x, topLeftY = finalPosition.y;
            topRightX = finalPosition.x + textureSize.x, topRightY = finalPosition.y;
            bottomLeftX = finalPosition.x, bottomLeftY = finalPosition.y + textureSize.y;
            bottomRightX = finalPosition.x + textureSize.x, bottomRightY = finalPosition.y + textureSize.y;

            CoordinateTransformation::ScreenToNdc(
                topLeftX, topLeftY, // TopLeftX, Y
                topRightX, topRightY, // TopRightX, Y
                bottomLeftX, bottomLeftY, // BottomRightX, Y,
                bottomRightX, bottomRightY, // BottomRightX, Y
                static_cast<float>(_viewPortSize.x), static_cast<float>(_viewPortSize.y)
            );
        }

        // Map/UnMap
        {
            void* data = _resource.vertexBuffer->Map();
            {
                if (data)
                {
                    const auto vertices = static_cast<SpriteVertex*>(data);

                    // Top left.
                    vertices[0].position = { topLeftX, topLeftY, 1.0f, 1.0f };
                    vertices[0].color = color;
                    vertices[0].texcoord = { 0, 0 };

                    // Top right.
                    vertices[1].position = { topRightX, topRightY, 1.0f, 1.0f };
                    vertices[1].color = color;
                    vertices[1].texcoord = { 1, 0 };

                    // Bottom Left
                    vertices[2].position = { bottomLeftX, bottomLeftY, 1.0f, 1.0f };
                    vertices[2].color = color;
                    vertices[2].texcoord = { 0, 1 };

                    // Bottom right
                    vertices[3].position = { bottomRightX, bottomRightY, 1.0f, 1.0f };
                    vertices[3].color = color;
                    vertices[3].texcoord = { 1, 1 };
                }
            }
            _resource.vertexBuffer->UnMap();
        }

        // Rendering pipeline
        {
            ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
            context->PSSetShaderResources(0, 1, _resource.texture->GetResource());
            // Bind buffer.
            {
                _resource.vertexBuffer->Bind();
            }

            // Bind shaders.
            {
                const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

                const std::optional<Ref<Shader>> shader = shaderLibrary->Find(ShaderLibrary::SPRITE_SHADER_PATH);
                if (!shader.has_value())
                {
                    DENTY_ASSERT(false, "Shader not found! (in Sprite::Render)");
                }

                shader.value()->constantBuffer->UpdateSubresource(context, &_constants);
                shader.value()->constantBuffer->BindAll(context, 0u);

                shader.value()->BindVertexShader(context);
                shader.value()->BindPixelShader(context);
            }

            context->Draw(4u, 0u);
        }
    }
}
