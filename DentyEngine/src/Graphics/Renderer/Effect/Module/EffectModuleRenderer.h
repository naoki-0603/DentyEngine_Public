#pragma once

#include "EffectModule.h"
#include "Graphics/Vertex.h"

#include "Graphics/Renderer/Material.h"

namespace DentyEngine
{
	class DXVertexBuffer;

	enum class EffectRenderingOrder : int32_t
    {
        None = -1,
        DistanceView = 0,
        MiddleDistance,
        CloseView,

        Max
    };

    struct EffectModuleRendererCPUContext
    {
    public:
        EffectModuleRendererCPUContext();

    public:
        ID3D11DeviceContext* dxContext;

        DXVertexBuffer* vertexBuffer;
    };

    struct EffectModuleRendererConstants
    {
    public:
        EffectModuleRendererConstants();

    public:
        // Vertex shader.
        Matrix world;

        // Pixel shader.
        Color albedoColor;
        Color specularColor;
        Color ambientColor;
        Color emissionColor;
        float metallic;
        float roughness;
        float kd;
        float ks;

        float ka;
        Vector3 padding;

        /*Material::TextureUsed textureUsed;*/
    };

    class EffectModuleRenderer final : public EffectModule
    {
    public:
        EffectModuleRenderer();
        EffectModuleRenderer(const EffectModuleRenderer& source);
        ~EffectModuleRenderer() override = default;

        void Initialize() override;

        void OnGui() override;

        void OnUpdateCPU(const Matrix& world);

        void OnRenderCPU(
            const EffectModuleRendererCPUContext& context
        );

        [[nodiscard]]
        EffectModuleType GetType() const override;

        [[nodiscard]]
        EffectRenderingOrder GetRenderingOrder() const;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<EffectModule>(this),
                    CEREAL_NVP(_material),
                    CEREAL_NVP(_renderingOrder)
                );
            }
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<EffectModule>(this),
                    CEREAL_NVP(_material),
                    CEREAL_NVP(_renderingOrder)
                );
            }
        }
    private:
        Ref<Material> _material;

        EffectModuleRendererConstants _constants;

        EffectRenderingOrder _renderingOrder;
    };
}

CEREAL_CLASS_VERSION(DentyEngine::EffectModuleRenderer, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::EffectModuleRenderer)
