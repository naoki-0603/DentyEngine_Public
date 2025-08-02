#pragma once

#include "System/Core/Object.h"

#include "Shader.h"

#include "Graphics/Color.h"

#include "System/Core/ID/MaterialID.h"

#include "Texture.h"

#include "System/Resource/Asset.h"

namespace DentyEngine
{
    enum class MaterialTextureIndex : int8_t
    {
	    None = -1,
        Albedo = 0,
        Metalness = 1,
        Normal = 2,
        Roughness = 3,
        Emissive = 4,
        AmbientOcclusion = 5,
        OptionalSlot1 = 6,
        OptionalSlot2 = 7,

        Max
    };

    enum class RenderingMode : int32_t
    {
	    None = -1,
        Opaque = 0,
        Transparent = 1,

        Max
    };

    struct MaterialConstants final
    {
    public:
        struct TextureUsed final
        {
        public:
            TextureUsed() noexcept;
            ~TextureUsed() noexcept = default;

        public:
            int32_t albedo;
            int32_t metalness;
            int32_t normal;
            int32_t roughness;

            int32_t emissive;
            int32_t ambientOcclusion;
            int32_t optionalSlot1;
            int32_t optionalSlot2;
        };
    public:
        MaterialConstants() noexcept;
        ~MaterialConstants() noexcept = default;

    public:
        Color baseColor;
        Color emissiveColor;

    	float baseRate;
        float roughness;
        float metallic;
        float specular;

        float emissiveIntensity;
        RenderingMode renderingMode;
        int32_t influenceHemiSphereLight;
        float padding;

        TextureUsed texturesUsed;
    };

    struct MaterialParameters final
    {
    public:
        MaterialParameters() noexcept;
        MaterialParameters(const MaterialParameters& source) noexcept;
        ~MaterialParameters() noexcept = default;
    public:
        Color baseColor;
        Color emissiveColor;

        float baseRate;
        float roughness;
        float metallic;
        float specular;
        float emissiveIntensity;

        RenderingMode renderingMode;

        bool influenceHemisphereLight;
    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(baseColor),
                CEREAL_NVP(emissiveColor),
                CEREAL_NVP(baseRate),
                CEREAL_NVP(roughness),
                CEREAL_NVP(metallic),
                CEREAL_NVP(specular),
                CEREAL_NVP(emissiveIntensity),
                CEREAL_NVP(renderingMode),
                CEREAL_NVP(influenceHemisphereLight)
            );
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version == 1u)
            {
                archive(
                    CEREAL_NVP(baseColor),
                    CEREAL_NVP(emissiveColor),
                    CEREAL_NVP(baseRate),
                    CEREAL_NVP(roughness),
                    CEREAL_NVP(metallic),
                    CEREAL_NVP(specular),
                    CEREAL_NVP(emissiveIntensity)
                );
            }
            else if (version == 2u)
            {
                archive(
                    CEREAL_NVP(baseColor),
                    CEREAL_NVP(emissiveColor),
                    CEREAL_NVP(baseRate),
                    CEREAL_NVP(roughness),
                    CEREAL_NVP(metallic),
                    CEREAL_NVP(specular),
                    CEREAL_NVP(emissiveIntensity),
                    CEREAL_NVP(renderingMode),
                    CEREAL_NVP(influenceHemisphereLight)
                );
            }
        }
    };

    struct MaterialInitData final
    {
    public:
        MaterialInitData() noexcept;
        ~MaterialInitData() noexcept = default;

    public:
        std::string name;

        FilePath filePath;

        Ref<Shader> shader;

        MaterialParameters parameters;

        bool isDefault;
    };

	class Material final :
		public Object,
		public Asset,
		public std::enable_shared_from_this<Material>
	{
	public:
		Material() noexcept;

		Material(const Material& source) noexcept;
        Material(const Material&& source) noexcept = delete;

		~Material() noexcept override = default;

        //
        // Member functions.
        //
        void Create(const MaterialInitData& data);

        void BindTextures() const;

        void RemoveTextureAt(MaterialTextureIndex index);
        void ReplaceTextureAt(Ref<Texture2D> texture, MaterialTextureIndex index);

        void ReplaceShaderHard(Ref<Shader> shader);
        void ReplaceShaderSoft(Ref<Shader> shader) noexcept;

        void SetTextureAt(Ref<Texture2D> texture, MaterialTextureIndex index);

		[[nodiscard]]
        std::optional<Ref<Texture2D>> GetTextureAt(MaterialTextureIndex index) const noexcept;

        [[nodiscard]]
        Ref<Shader> GetShader() const noexcept;

        [[nodiscard]]
        bool HasTextureAt(MaterialTextureIndex index) const;

        [[nodiscard]]
        bool IsEqual(const Ref<Material>& target) const;

        [[nodiscard]]
        bool IsDefault() const noexcept;

        //
        // Operators
        //
        Material& operator=(const Material& source) = delete;
        Material& operator=(const Material&& source) noexcept = delete;

        //
        // Override member functions (Object class)
        //
        void OnSerialized() const override;
        void OnDeserialized() override;

        //
        // Override member functions (Asset class)
        //
        void Save(bool isUpdate = false) const override;
        void Load(const std::filesystem::path& filePath) override;

        void Rename(std::string_view newName, bool deleteOldFile) override;

		void OnReload() override;

        void Clone(const Ref<Asset>& asset) override;

        [[nodiscard]]
        AssetType GetAssetType() const noexcept override;
	public:
        static void Setup();

        [[nodiscard]]
        static Ref<Material> GetStaticDefaultMaterial() noexcept;

        [[nodiscard]]
        static Ref<Material> GetSkinnedDefaultMaterial() noexcept;

        [[nodiscard]]
        static Ref<Material> GetStaticShadowDefaultMaterial() noexcept;

        [[nodiscard]]
        static Ref<Material> GetSkinnedShadowDefaultMaterial() noexcept;

        [[nodiscard]]
        static Ref<Texture2D> GetDefaultThumbNail() noexcept;
	public:
        MaterialParameters parameters;

	public:
        //
        // Static variables.
        //
        static const String ADD_FILENAME_AT_END;
        static const std::array<std::string, 2u> RENDERING_MODES;
	private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version == 1u)
            {
                archive(
                    cereal::base_class<Object>(this),
                    cereal::base_class<Asset>(this),
                    CEREAL_NVP(parameters),
                    CEREAL_NVP(_texturesFilePath),
                    CEREAL_NVP(_shaderFilePath),
                    CEREAL_NVP(_default)
                );
            }

            OnSerialized();
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version == 1u)
            {
                archive(
                    cereal::base_class<Object>(this),
                    cereal::base_class<Asset>(this),
                    CEREAL_NVP(parameters),
                    CEREAL_NVP(_texturesFilePath),
                    CEREAL_NVP(_shaderFilePath),
                    CEREAL_NVP(_default)
                );
            }

            OnDeserialized();
        }
	private:
        std::vector<Ref<Texture2D>> _textures;
        std::vector<ID3D11ShaderResourceView*> _shaderResourceViews;

        // Serialize data.
        std::vector<FilePath> _texturesFilePath;

        Ref<Shader> _shader;
        FilePath _shaderFilePath;

        bool _default;
	private:
        static Ref<Material> _staticDefaultMaterial;
        static Ref<Material> _skinnedDefaultMaterial;
        static Ref<Material> _staticShadowDefaultMaterial;
        static Ref<Material> _skinnedShadowDefaultMaterial;
        static Ref<Texture2D> _defaultThumbnail;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::MaterialParameters, 2u)
CEREAL_CLASS_VERSION(DentyEngine::Material, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Material)