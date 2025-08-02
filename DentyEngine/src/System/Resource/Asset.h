#pragma once

#include "System/Core/DFilePath.h"

#include "System/Core/ID/UUID.h"

namespace DentyEngine
{
	class Texture2D;

	class Object;

	class Asset
	{
	public:
		enum class AssetType : int32_t
		{
			None = -1,
			Material = BIT(0),
			Shader = BIT(1),
			Mesh = BIT(2),
			CollisionMesh = BIT(3),
			ApproximationCollisionMesh = BIT(4),
			Texture = BIT(5),
			Prefab = BIT(6),
			Scene = BIT(7),
			Animation = BIT(8),
			Controller = BIT(9),
			SceneTree = BIT(10),
			Avatar = BIT(11),
			Skybox = BIT(12),
			Sprite = BIT(13),
			Script = BIT(14),
			CubeMaterial = BIT(15),

			Max = 16
		};
	public:
		Asset();
		Asset(const Asset& source);
		virtual ~Asset() = default;

		virtual void Save(bool isUpdate = false) const = 0;
		virtual void Load(const std::filesystem::path& filePath) = 0;

		virtual void Rename(std::string_view newName, bool deleteOldFile = true) = 0;

		virtual void OnReload() = 0;

		virtual void Clone(const Ref<Asset>& asset) = 0;

		virtual void ReGenerateID();

		//
		// Event functions.
		//

		//
		// Setter
		//
		void SetFilePath(std::string_view filePath) { _filePath = filePath; }
		void SetFilePathToSerialized(std::string_view filePath) { _filePathToSerialized = filePath; }
		void SetPreviewImage(const Ref<Texture2D>& previewImage);

		//
		// Getter
		//
		[[nodiscard]]
		virtual AssetType GetAssetType() const = 0;

		[[nodiscard]]
		virtual Ref<Texture2D> GetPreviewImage() const { return _previewImage; }

		[[nodiscard]]
		const FilePath& GetFilePath() const { return _filePath; }

		[[nodiscard]]
		const FilePath& GetFilePathToSerialized() const { return _filePathToSerialized; }

		[[nodiscard]]
		bool HasPreviewImage() const { return (_previewImage != nullptr); }
	protected:
		// If doesn't exists preview image, should be nullptr.
		Ref<Texture2D> _previewImage;

		// For serialize.
		FilePath _previewImageFilePath;

		FilePath _filePath;
		FilePath _filePathToSerialized;

		std::unordered_map<DentyEngine::UUID, std::weak_ptr<Object>> _referenceObjects;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(_previewImageFilePath),
				CEREAL_NVP(_filePath),
				CEREAL_NVP(_filePathToSerialized)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(_previewImageFilePath),
				CEREAL_NVP(_filePath),
				CEREAL_NVP(_filePathToSerialized)
			);
		}
	};

	DEFINE_ENUM_CLASS_BIT_OPERATORS(Asset::AssetType)
}

CEREAL_CLASS_VERSION(DentyEngine::Asset, 1u)