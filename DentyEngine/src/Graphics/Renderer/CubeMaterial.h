#pragma once

#include "System/Core/Object.h"
#include "System/Resource/Asset.h"

namespace DentyEngine
{
	class CubeMap;
	class Shader;

	struct CubeMaterialInitData final
	{
	public:
		CubeMaterialInitData() noexcept;
		~CubeMaterialInitData() noexcept = default;

	public:
		std::string name;

		mutable FilePath filePath;
		mutable FilePath textureFilePath;

		Ref<Shader> shader;
	};

	// This material class for using cube map textures.
	class CubeMaterial final :
		public Object,
		public Asset,
		public std::enable_shared_from_this<CubeMaterial>
	{
	public:
		CubeMaterial() noexcept;

		CubeMaterial(const CubeMaterial& source) noexcept;
		CubeMaterial(const CubeMaterial&&) noexcept = delete;

		~CubeMaterial() noexcept override;

		//
		// Member functions.
		//
		void Create(const CubeMaterialInitData& data);

		void RemoveTexture() noexcept;
		void ReplaceTexture(Ref<CubeMap> cubeMap);

		//
		// Setter 
		//
		void SetTexture(Ref<CubeMap> cubeMap);

		//
		// Getter
		//
		[[nodiscard]]
		Ref<Shader> GetShader() const noexcept;

		[[nodiscard]]
		Ref<CubeMap> GetTexture() const noexcept;

		[[nodiscard]]
		bool HasTexture() const noexcept;

		//
		// Operators
		//
		CubeMap& operator=(const CubeMap&) = delete;
		CubeMap& operator=(const CubeMap&&) noexcept = delete;

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
		static Ref<Texture2D> GetDefaultThumbnail() noexcept;
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
					CEREAL_NVP(_textureFilePath),
					CEREAL_NVP(_shaderFilePath)
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
					CEREAL_NVP(_textureFilePath),
					CEREAL_NVP(_shaderFilePath)
				);
			}

			OnDeserialized();
		}

	private:
		Ref<CubeMap> _texture;
		FilePath _textureFilePath;

		Ref<Shader> _shader;
		FilePath _shaderFilePath;

	private:
		static Ref<Texture2D> _defaultThumbnail;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::CubeMaterial, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::CubeMaterial)