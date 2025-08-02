#pragma once

#include "System/Core/DString.h"
#include "InputLayout.h"
#include "System/Core/Object.h"

#include "System/Core/ID/ShaderID.h"

#include "DXShaders/VertexShader.h"
#include "DXShaders/HullShader.h"
#include "DXShaders/DomainShader.h"
#include "DXShaders/GeometryShader.h"
#include "DXShaders/PixelShader.h"
#include "DXShaders/ComputeShader.h"

#include "System/Resource/Asset.h"

namespace DentyEngine
{
    struct ShaderPathHolder
    {
        std::string vertexShaderFilePath;
        std::string hullShaderFilePath;
        std::string domainShaderFilePath;
        std::string geometryShaderFilePath;
        std::string pixelShaderFilePath;
        std::vector<std::string> computeShaderFilePaths;

    private:
        friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
        {
            archive(
                CEREAL_NVP(vertexShaderFilePath),
                CEREAL_NVP(hullShaderFilePath),
                CEREAL_NVP(domainShaderFilePath),
                CEREAL_NVP(geometryShaderFilePath),
                CEREAL_NVP(pixelShaderFilePath),
                CEREAL_NVP(computeShaderFilePaths)
            );
        }

        template <class Archive>
        void load(Archive& archive, [[maybe_unused]] const uint32_t version)
        {
            archive(
                CEREAL_NVP(vertexShaderFilePath),
                CEREAL_NVP(hullShaderFilePath),
                CEREAL_NVP(domainShaderFilePath),
                CEREAL_NVP(geometryShaderFilePath),
                CEREAL_NVP(pixelShaderFilePath),
                CEREAL_NVP(computeShaderFilePaths)
            );
        }
    };

    class ConstantBuffer;
    class Shader final : public Asset, public Object, public std::enable_shared_from_this<Shader>
    {
        friend class BuildInShader;
    public:
        Shader();
        ~Shader() override = default;

        // Operators.
        Shader& operator=(const Shader& source);
        
        // If constant buffer size is zero, constant buffer does not create.
        void Create(ID3D11Device* device, const std::string& name, size_t constantBufferSize);

        void OnEvent(Event* e) override;
        void OnSerialized() const override;
        void OnDeserialized() override;

        void Save(bool isUpdate = false) const override;
        void Load(const std::filesystem::path& filePath) override;

    	[[maybe_unused]]
        void Rename(std::string_view newName, bool deleteOldFile = true) override;

        void OnReload() override;

    	//void UpdateAssetFile(EventAssetUpdatedOperationType type) override;
        void Clone(const Ref<Asset>& asset) override;

        //void OnRename(std::string_view newName) override;

        void CreateVertexShader(ID3D11Device* device, const std::string& filePath, const D3D11_INPUT_ELEMENT_DESC* elementDesc, size_t elementNum);
        void CreateHullShader(ID3D11Device* device, const std::string& filePath);
        void CreateDomainShader(ID3D11Device* device, const std::string& filePath);
        void CreateGeometryShader(ID3D11Device* device, const std::string& filePath);
        void CreatePixelShader(ID3D11Device* device, const std::string& filePath);
        void CreateComputeShader(ID3D11Device* device, const std::string& filePath);

        void BindVertexShader(ID3D11DeviceContext* context);
        void BindHullShader(ID3D11DeviceContext* context);
        void BindDomainShader(ID3D11DeviceContext* context);
        void BindGeometryShader(ID3D11DeviceContext* context);
        void BindPixelShader(ID3D11DeviceContext* context);
        void BindComputeShader(ID3D11DeviceContext* context, const std::string& fileName);

        void ReCompile();

        //
        // Getter
        //

        [[nodiscard]]
        AssetType GetAssetType() const override { return AssetType::Shader; }

        [[nodiscard]]
        ShaderID GetShaderID() const { return _id; }

        [[nodiscard]]
        const ShaderPathHolder& GetShaderPathHolder() const { return _pathHolder; }

        //
        // Static functions.
        //
        [[nodiscard]]
		static Ref<Texture2D> GetAssetTexture() { return _shaderAssetTexture; }

        static void LoadAssetTexture();
    public:
        Ref<ConstantBuffer> constantBuffer;

        //
        // Static variables.
        //
        static const char* const DEFAULT_VS_TARGET;
        static const char* const DEFAULT_PS_TARGET;
        static const char* const DEFAULT_CS_TARGET;
        static const char* const DEFAULT_HS_TARGET;
        static const char* const DEFAULT_DS_TARGET;
        static const char* const DEFAULT_GS_TARGET;
    private:
    	friend class cereal::access;

        template <class Archive>
        void save(Archive& archive, const uint32_t version) const
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Object>(this),
                    cereal::base_class<Asset>(this),
                    CEREAL_NVP(_id),
                    CEREAL_NVP(_pathHolder),
                    CEREAL_NVP(_inputLayoutType),
                    CEREAL_NVP(_constantBufferSize)
                );
            }

            OnSerialized();
        }

        template <class Archive>
        void load(Archive& archive, const uint32_t version)
        {
            if (version <= 1u)
            {
                archive(
                    cereal::base_class<Object>(this),
                    cereal::base_class<Asset>(this),
                    CEREAL_NVP(_id),
                    CEREAL_NVP(_pathHolder),
                    CEREAL_NVP(_inputLayoutType),
                    CEREAL_NVP(_constantBufferSize)
                );
            }

            OnDeserialized();
        }

        void CreateBuildIn(ID3D11Device* device, const std::string& name, size_t constantBufferSize);

        ComRef<ID3DBlob> CompileShader(const std::string& filePath, const char* shaderTarget);

    private:
        ShaderID _id;
        ShaderPathHolder _pathHolder;

        // For deserialize.
        size_t _constantBufferSize;

        Ref<VertexShader> _vertexShader;
        Ref<HullShader> _hullShader;
        Ref<DomainShader> _domainShader;
        Ref<GeometryShader> _geometryShader;
        Ref<PixelShader> _pixelShader;

        ComRef<ID3D11InputLayout> _inputLayout;

        // For deserialize.
        InputLayout::Type _inputLayoutType;

    	std::unordered_map<std::string, Ref<ComputeShader>> _computeShaders;
    private:
        //
        // Static variables.
        //

        static Ref<Texture2D> _shaderAssetTexture;
    };
};

CEREAL_CLASS_VERSION(DentyEngine::ShaderPathHolder, 1u)
CEREAL_CLASS_VERSION(DentyEngine::Shader, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Shader)