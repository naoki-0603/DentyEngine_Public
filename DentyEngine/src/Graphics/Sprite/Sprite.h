#pragma once

#include "System/Core/Object.h"

#include "SpriteInfo.h"

#include "Graphics/Vertex.h"

#include "System/Core/ID/EntityID.h"

namespace DentyEngine
{
    namespace Utils
    {
        [[nodiscard]]
        static Vector2 CalculatePivotBasedPosition(const Vector2& position, const Vector2& textureSize, SpritePivot pivot);
    }

    class DXVertexBuffer;

    class Texture2D;

    struct SpriteResource
    {
        SpriteResource();

        void Create(Ref<Texture2D> paramTexture, SpritePivot paramPivot);
        void CreateBuffer();

        SpritePivot pivot;

        Ref<Texture2D> texture;

        std::vector<SpriteVertex> vertices;
        Ref<DXVertexBuffer> vertexBuffer;
    };

    struct SpriteConstants
    {
        SpriteConstants();

        EntityID id;
        Vector3 padding;
    };

    class Sprite final : public Object
    {
    public:
        Sprite();
        ~Sprite() override = default;

        //
        // Create function.
        //
        void Create(Ref<Texture2D> texture, SpritePivot pivot = SpritePivot::Center);

        //
        // Update function.
        //
        void Update(EntityID id, const Vector2I& viewPortSize);

        //
        // Render functions.
        //
        void Render(const Vector2& position, const Color& color = Color::WHITE);
        void Render(const Vector2& position, float scale, float rotation, const Color& color = Color::WHITE);
    private:
        SpriteResource _resource;
        SpriteConstants _constants;

        Vector2I _viewPortSize;
    };
}
