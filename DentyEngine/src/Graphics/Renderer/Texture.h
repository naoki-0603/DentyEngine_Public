#pragma once

#include "System/Core/DString.h"
#include "System/Core/Object.h"

#include "System/Resource/Asset.h"

#include "System/Core/ID/TextureID.h"

namespace DentyEngine
{
	class Texture : public Asset, public Object
	{
	public:
		enum class Type
		{
			None = -1,
			Texture2D,
			CubeMap,

			Max
		};
	public:
		Texture();
		Texture(const Texture&) = default;
		~Texture() override = default;

		Texture& operator=(const Texture& source);

		void OnEvent(Event* e) override;
		void OnSerialized() const override;
		void OnDeserialized() override;

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Rename(std::string_view newName, bool deleteOldFile = true) override;
		//void UpdateAssetFile(EventAssetUpdatedOperationType type) override {}
		void Clone(const Ref<Asset>& asset) override;

		void OnReload() override;

		//void OnRename(std::string_view newName) override;

		//
		// Setter
		//
		void SetAlias(std::string_view alias) { _alias = alias; }

		//
		// Getter
		//
		[[nodiscard]]
		AssetType GetAssetType() const override { return AssetType::Texture; }

		[[nodiscard]]
		DXGI_FORMAT GetFormat() const { return _format; }

		[[nodiscard]]
		TextureID GetTextureID() const { return _textureID; }

		[[nodiscard]]
		Type GetType() const { return _type; }

		[[nodiscard]]
		const Vector2& GetSize() const { return _size; }

		[[nodiscard]]
		const std::string& GetAlias() const { return _alias; }

		[[nodiscard]]
		ID3D11ShaderResourceView* const* GetResource() const { return _shaderResourceView.GetAddressOf(); }

		[[nodiscard]]
		ComRef<ID3D11ShaderResourceView> GetResourceAsComPtr() const { return _shaderResourceView; }

		[[nodiscard]]
		bool IsDummyTexture() const { return _isDummyTexture; }

		//
		// Static functions.
		//
		static bool SaveTextureToFile(ID3D11DeviceContext* context, ID3D11Resource* buffer, std::string_view filePath, const Vector2& size, DXGI_FORMAT format);
	public:

		//
		// Static variables.
		//
		static const String ADD_FILENAME_AT_END;
	protected:
		DXGI_FORMAT _format;

		TextureID _textureID;

		Type _type;

		// Priority is higher than original name.
		std::string _alias;

		Vector2 _size;

		ComRef<ID3D11ShaderResourceView> _shaderResourceView;

		bool _isDummyTexture;
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
				CEREAL_NVP(_format),
				CEREAL_NVP(_textureID),
				CEREAL_NVP(_type),
				CEREAL_NVP(_alias),
				CEREAL_NVP(_size),
				CEREAL_NVP(_isDummyTexture)
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
				CEREAL_NVP(_format),
				CEREAL_NVP(_textureID),
				CEREAL_NVP(_type),
				CEREAL_NVP(_alias),
				CEREAL_NVP(_size),
				CEREAL_NVP(_isDummyTexture)
			);
			}

			OnDeserialized();
		}
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Texture, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Texture)