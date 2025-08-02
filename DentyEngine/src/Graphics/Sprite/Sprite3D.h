#pragma once

#include "System/Core/Object.h"

#include "Graphics/Vertex.h"

#include "System/Core/ID/EntityID.h"

namespace DentyEngine
{
    class Texture2D;

    class DXVertexBuffer;
    class DXIndexBuffer;
    class Shader;

    struct Sprite3DCommonResource final
    {
    public:
        Sprite3DCommonResource();
        ~Sprite3DCommonResource() noexcept = default;

    public:
        std::array<SpriteVertex, 4u> vertices;
        std::array<uint32_t, 6u> indices;

        Ref<DXVertexBuffer> vertexBuffer;
        Ref<DXIndexBuffer> indexBuffer;
        Ref<Shader> shader;
    };

    struct Sprite3DResource
    {
        Sprite3DResource();

        void Create(Ref<Texture2D> paramTexture);

        Ref<Texture2D> texture;
    };

    struct Sprite3DConstants
    {
        Sprite3DConstants();

        EntityID id;
    	Vector3 padding;

        Color color;

        Matrix world;
    };

    class Sprite3D final : public Object
    {
    public:
        Sprite3D();
        ~Sprite3D() override = default;

        //
        // Create function.
        //
        void Create(Ref<Texture2D> texture);

        //
        // Update function.
        //
        //void Update(EntityID id, const Color& color, const Vector4& positionInWorld);
        void Update(const Matrix& world, const Color& color, EntityID id);

        //
        // Render functions.
        //
        void Render();

        static void Setup();
    private:
        Sprite3DResource _resource;
        Sprite3DConstants _constants;

    private:
        static Sprite3DCommonResource _commonResource;
    };
}
