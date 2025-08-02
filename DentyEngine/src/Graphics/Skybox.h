#pragma once

#include "Vertex.h"

#include "System/Core/Object.h"

#include "System/Resource/Asset.h"

namespace DentyEngine
{
	class CubeMap;
	class CubeMaterial;

	class DXVertexBuffer;
	class DXIndexBuffer;

	class Skybox final : public Object, public Asset, public std::enable_shared_from_this<Skybox>
	{
	public:
		struct Constants
		{
			Matrix world;
		};
	public:
		Skybox();
		~Skybox() override = default;

		void Create(const FilePath& cubeMapFilePath);

		void OnEvent(Event* e) override;
		void OnSerialized() const override;
		void OnDeserialized() override;

		void Render(ID3D11DeviceContext* context);

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Rename(std::string_view newName, bool deleteOldFile) override;

		void OnReload() override;

		void Clone(const Ref<Asset>& asset) override;

		void BindCubeMap();

		//
		// Getter
		//
		[[nodiscard]]
		AssetType GetAssetType() const override { return AssetType::Skybox; }

		//
		// Static functions.
		//
		[[nodiscard]]
		static Ref<Texture2D> GetAssetTexture() { return _skyboxAssetTexture; }

		static void LoadAssetTexture();
	private:
		void CreateBuffers();

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
					CEREAL_NVP(_vertices),
					CEREAL_NVP(_indices),
					CEREAL_NVP(_cubeMaterialFilePath)
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
					CEREAL_NVP(_vertices),
					CEREAL_NVP(_indices),
					CEREAL_NVP(_cubeMaterialFilePath)
			    ); 
			}
			

			OnDeserialized();
		}
	private:
		// For serialize.
		std::vector<Vertex> _vertices;

		// For serialize.
		std::vector<uint32_t> _indices;

		// Doesn't serialize.
		Ref<CubeMaterial> _cubeMaterial;

		// For serialize instead of _cubeMaterial.
		FilePath _cubeMaterialFilePath;

		// Doesn't serialize.
		Ref<DXVertexBuffer> _vertexBuffer;

		// Doesn't serialize.
		Ref<DXIndexBuffer> _indexBuffer;

		// Doesn't serialize.
		Constants _constants;
	private:
		static Ref<Texture2D> _skyboxAssetTexture;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Skybox, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Skybox)