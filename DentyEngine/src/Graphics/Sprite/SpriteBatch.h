#pragma once

#include "System/Core/Object.h"

#include "Graphics/Vertex.h"

#include "SpriteInfo.h"

#include "Graphics/States/BlendState.h"
#include "Graphics/States/DepthStencilState.h"
#include "Graphics/States/RasterizerState.h"
#include "Graphics/States/SamplerState.h"

#include "System/Core/ID/EntityID.h"

//
// References
//

// https://github.com/microsoft/DirectXTK/wiki/SpriteBatch
// https://setuna-kanata.hatenadiary.org/entry/20100721/1279731858

namespace DentyEngine
{
    class DXVertexBuffer;
    class DXIndexBuffer;

    class Texture2D;

    struct SpriteBatchResource
    {
        SpriteBatchResource();

        void Create(Ref<Texture2D> texture2D, SpriteSortMode mode = SpriteSortMode::Deferred, bool isWorld = false);

        void CreateBuffers();
    public:
        Ref<Texture2D> texture;

        std::vector<SpriteVertex> vertices;
        std::vector<uint32_t> indices;

        Ref<DXVertexBuffer> vertexBuffer;
        Ref<DXIndexBuffer> indexBuffer;
    };

    struct SpriteBatchConstants
    {
        SpriteBatchConstants();

        EntityID id;
        Vector3 padding;

        // If isWorld is true, set world orientation.
        // otherwise, set identity.
        Matrix world;
    };

    class SpriteBatch final : public Object
    {
    public:
        SpriteBatch();
        ~SpriteBatch() override = default;

        //
        // Create functions
        //
        void Create(Ref<Texture2D> texture2D, SpriteSortMode mode = SpriteSortMode::Deferred, bool isWorld = false);

        //
        // Begin functions.
        //
        void Begin(BlendState::Type blendStateType = BlendState::Type::Alpha,
                DepthStencilState::Type depthStencilType = DepthStencilState::Type::DepthOnStencilOn,
                RasterizerState::Type rasterizerStateType = RasterizerState::Type::FillCullBack,
                SamplerState::Type samplerStateType = SamplerState::Type::PointWrap
        );

        //
        // Render functions.
        //
        void RenderOnScreen(const Vector2& position, float rotation = 0.0f, float scale = 1.0f, const Color& color = Color::WHITE);
        void RenderOnWorld(const Vector3& position, const Color& color = Color::WHITE);

        //
        // End function.
        //
        void End();
    public:
        static constexpr uint32_t MAX_IMMEDIATE_SPRITE_COUNT = 1u;
        static constexpr uint32_t MAX_SPRITE_COUNT = 2048u;

    private:
        void PrepareForRendering();

        void RenderBatchOnWorld(const Vector3& position, const Color& color, uint32_t spriteCount);
        void RenderBatchOnScreen(const Vector2& position, float rotation , float scale, const Color& color, uint32_t spriteCount);

        void RenderSpriteOnWorld(const Vector3& position, const Vector2& textureSize, const Color& color);
        void RenderSpriteOnScreen(const Vector2& position, const Vector2& textureSize, float rotation, float scale, const Color& color);
    private:
        SpriteBatchResource _resource;
        SpriteBatchConstants _constants;

        uint32_t _indexBufferOffset;

        SpriteSortMode _sortMode;

        BlendState::Type _blendStateType;
        DepthStencilState::Type _depthStencilStateType;
        RasterizerState::Type _rasterizerStateType;
        SamplerState::Type _samplerStateType;

        bool _isWorld;
        bool _isInImmediateMode;
        bool _isBeginCalled;
    };
}
